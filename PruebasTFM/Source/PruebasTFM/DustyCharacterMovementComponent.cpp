// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyCharacterMovementComponent.h"
#include "DustyCharacter.h"
#include "Components/CapsuleComponent.h"

UDustyCharacterMovementComponent::UDustyCharacterMovementComponent()
{
	bOrientRotationToMovement = true; // Character moves in the direction of input...	
	RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	bCanWalkOffLedges = true;

	NavAgentProps.bCanCrouch = true;
}

void UDustyCharacterMovementComponent::SetSprinting(bool NewSprinting)
{
	bIsSprinting = NewSprinting;
}

void UDustyCharacterMovementComponent::SetCrouched()
{
	bWantsToCrouch = !bWantsToCrouch;	
}


bool UDustyCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode Mode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == Mode;
}

void UDustyCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	if (MovementMode == MOVE_Walking)
	{
		if (bIsSprinting)
		{
			MaxWalkSpeed = Max_SprintSpeed;
		}
		else
		{
			MaxWalkSpeed = Max_WalkSpeed;
		}
	}

	bPrevWantsToCrouch = bWantsToCrouch;


}

void UDustyCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
	{
		ExitSlide();
	}

	Super::UpdateCharacterStateBeforeMovement(DeltaSeconds);
}

void UDustyCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	DustyCharacterOwner = Cast<ADustyCharacter>(GetOwner());
}

void UDustyCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	switch (CustomMovementMode)
	{
		case CMOVE_Slide:
			PhysSlide(deltaTime, Iterations);
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("Unknown Custom Movement Mode"));
	}
}

bool UDustyCharacterMovementComponent::IsMovingOnGround() const
{
	//Hay que overraidear este metodo porque el super moving on ground no tiene en cuenta los custom movement modes
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide);
}

bool UDustyCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

void UDustyCharacterMovementComponent::EnterSlide()
{
	bSliding = true;
	bWantsToCrouch = true;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;
	SetMovementMode(MOVE_Custom, CMOVE_Slide);
//	FHitResult PotentialSlideSurface;
//	if (Velocity.SizeSquared() > pow(Min_SlideSpeed, 2) && GetSlideSurface(PotentialSlideSurface))
//	{
//
//		
//	}
}

void UDustyCharacterMovementComponent::ExitSlide()
{
	bSliding = false;
	bWantsToCrouch = false;

	//Despues de deslizar vuelves a poner la capsula en vertical
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(UpdatedComponent->GetForwardVector().GetSafeNormal2D(), FVector::UpVector).ToQuat();
	FHitResult Hit;
	SafeMoveUpdatedComponent(FVector::ZeroVector, NewRotation, true, Hit);

	SetMovementMode(MOVE_Walking);
}

void UDustyCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	RestorePreAdditiveRootMotionVelocity();

	FHitResult SurfaceHit;
	if (!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(Min_SlideSpeed,2))
	{
		ExitSlide();
		//Dejas de deslizar entonces cambias de modo de movimiento y dejas de estar en slide
		StartNewPhysics(deltaTime, Iterations);
		return;
	}

	//Ya estamos deslizando
	Velocity += SlideGravityForce * FVector::DownVector * deltaTime;

	//Deslizandome no puedo moverme para adelante ni atrás
	//Imaginen un joystick, si lo muevo hacia arriba/abajo, el producto escalar con el right vector es 0, son perpendiculares, entonces se ignora.
	//Si lo muevo en diagonal o hacia los lados, el producto escalar es >0.5, entonces lo aplicamos. 
	//El project es por si lo movemos en diagonal, nos quedamos solo con la parte right de la dirección que estamos aplicando
	if (FMath::Abs(FVector::DotProduct(Acceleration.GetSafeNormal(), UpdatedComponent->GetRightVector())) > 0.5)
	{
		Acceleration = Acceleration.ProjectOnTo(UpdatedComponent->GetRightVector());
	}
	else
	{
		Acceleration = FVector::ZeroVector;
	}

	//Calculo de la velocidad
	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		CalcVelocity(deltaTime, SlideFriction, true, GetMaxBrakingDeceleration());
	}
	ApplyRootMotionToVelocity(deltaTime);

	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FQuat OldRotation = UpdatedComponent->GetComponentRotation().Quaternion();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime; // dx = v * dt
	FVector VelPlaneDir = FVector::VectorPlaneProject(Velocity, SurfaceHit.ImpactNormal).GetSafeNormal(); 
	FQuat NewRotation = FRotationMatrix::MakeFromXZ(VelPlaneDir, SurfaceHit.Normal).ToQuat();

	//Esta linea mueve al personaje
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		HandleImpact(Hit, deltaTime, Adjusted);

		// Esta también es importante
		SlideAlongSurface(Adjusted, 1.f - Hit.Time, Hit.Normal, Hit, true);
	}

	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(Min_SlideSpeed, 2))
	{
		ExitSlide();
	}

	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

}

bool UDustyCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	// El componente que se actualiza, aka el root de base
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + DustyCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DustyCharacterOwner->GetIgnoreCharacterCollisionParams());

}
