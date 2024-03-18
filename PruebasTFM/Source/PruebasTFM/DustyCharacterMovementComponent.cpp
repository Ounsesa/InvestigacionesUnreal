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

void UDustyCharacterMovementComponent::CrouchPressed()
{
	bWantsToCrouch = !bWantsToCrouch;	
	bCrouching = bWantsToCrouch;


	GetWorld()->GetTimerManager().SetTimer(ProneTimerHandle, this, &UDustyCharacterMovementComponent::TryEnterProne, ProneHoldDuration, false);
}

void UDustyCharacterMovementComponent::CrouchReleased()
{
	GetWorld()->GetTimerManager().ClearTimer(ProneTimerHandle);
}


bool UDustyCharacterMovementComponent::IsCustomMovementMode(ECustomMovementMode Mode) const
{
	return MovementMode == MOVE_Custom && CustomMovementMode == Mode;
}
bool UDustyCharacterMovementComponent::IsMovementMode(EMovementMode InMovementMode) const
{
	return InMovementMode == MovementMode;
}


void UDustyCharacterMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);
	bPrevWantsToCrouch = bWantsToCrouch;
}

void UDustyCharacterMovementComponent::UpdateCharacterStateBeforeMovement(float DeltaSeconds)
{
	if (IsCustomMovementMode(CMOVE_Slide) && !bWantsToCrouch)
	{
		SetMovementMode(MOVE_Walking);
	}

	if (bPrevWantsToProne)
	{
		if (CanProne())
		{
			SetMovementMode(MOVE_Custom, CMOVE_Prone);
		}
		bPrevWantsToProne = false;
	}
	if (IsCustomMovementMode(CMOVE_Prone) && !bWantsToCrouch)
	{
		SetMovementMode(MOVE_Walking);
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
		case CMOVE_Prone:
			PhysProne(deltaTime, Iterations);
			break;
		default:
			UE_LOG(LogTemp, Fatal, TEXT("Unknown Custom Movement Mode"));
	}
}

void UDustyCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Slide)
	{
		ExitSlide();
	}

	if (PreviousMovementMode == MOVE_Custom && PreviousCustomMode == CMOVE_Prone)
	{
		ExitProne();
	}

	if (IsCustomMovementMode(CMOVE_Slide))
	{
		EnterSlide();
	}
	if (IsCustomMovementMode(CMOVE_Prone))
	{
		EnterProne(PreviousMovementMode, (ECustomMovementMode)PreviousCustomMode);
	}

	if (IsFalling())
	{
		bOrientRotationToMovement = true;
	}

	
}


bool UDustyCharacterMovementComponent::IsMovingOnGround() const
{
	//Hay que overraidear este metodo porque el super moving on ground no tiene en cuenta los custom movement modes
	return Super::IsMovingOnGround() || IsCustomMovementMode(CMOVE_Slide) || IsCustomMovementMode(CMOVE_Prone);
}

bool UDustyCharacterMovementComponent::CanCrouchInCurrentState() const
{
	return Super::CanCrouchInCurrentState() && IsMovingOnGround();
}

float UDustyCharacterMovementComponent::GetMaxSpeed() const
{
	if (IsMovementMode(MOVE_Walking) && bIsSprinting && !IsCrouching()) return Max_SprintSpeed;

	if (MovementMode != MOVE_Custom) return Super::GetMaxSpeed();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return Min_SlideSpeed;
	case CMOVE_Prone:
		return ProneMaxSpeed;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
			return -1.f;
	}
}

float UDustyCharacterMovementComponent::GetMaxBrakingDeceleration() const
{
	if (MovementMode != MOVE_Custom) return Super::GetMaxBrakingDeceleration();

	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		return BrakingDecelerationSliding;
	case CMOVE_Prone:
		return BrakingDecelerationProne;
	default:
		UE_LOG(LogTemp, Fatal, TEXT("Invalid Movement Mode"))
			return -1.f;
	}
}

void UDustyCharacterMovementComponent::EnterSlide()
{
	bWantsToCrouch = true;
	bOrientRotationToMovement = false;
	Velocity += Velocity.GetSafeNormal2D() * SlideEnterImpulse;

	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);


	//Animation purposes
	bSliding = true;
}

void UDustyCharacterMovementComponent::CustomProne()
{
	bProning = !bProning;
	if (bProning)
	{
		bWantsToCrouch = true;
		DustyCharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(DustyCharacterOwner->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 4);

	}
	else
	{
		bWantsToCrouch = false;
		DustyCharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(DustyCharacterOwner->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	}
}



void UDustyCharacterMovementComponent::ExitSlide()
{
	bWantsToCrouch = false;
	bOrientRotationToMovement = true;

	//Animation purposes
	bSliding = false;
	bCrouching = false;
}

void UDustyCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}


	//Si no hay superficie para deslizar o la velocidad es muy baja, salimos del slide
	FHitResult SurfaceHit;
	if (!GetSlideSurface(SurfaceHit) || Velocity.SizeSquared() < pow(Min_SlideSpeed,2))
	{
		SetMovementMode(MOVE_Walking);
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
		CalcVelocity(deltaTime, SlideFriction, false, GetMaxBrakingDeceleration());
	}

	//Si la animación tiene root motion, se aplica, creo 
	//ApplyRootMotionToVelocity(deltaTime);

	//Cada vez que en un mismo frame se aplica un movimiento, se aumenta el contador de iteraciones
	Iterations++;
	bJustTeleported = false;

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	FHitResult Hit(1.f);
	FVector Adjusted = Velocity * deltaTime; // dx = v * dt

	FQuat NewRotation = FRotationMatrix::MakeFromXZ(Velocity.GetSafeNormal2D(), FVector::UpVector).ToQuat();

	//Esta linea mueve al personaje, esta es la importante
	SafeMoveUpdatedComponent(Adjusted, NewRotation, true, Hit);

	//Esto es lo que permite deslizarnos. Si hemos chocado con algo, es que hay una superficie debajo, entonces deslizamos
	if (Hit.Time < 1.f)
	{
		//Esto no sé mucho
		HandleImpact(Hit, deltaTime, Adjusted);

		// Esto es lo que hace deslizemos por esa superficie que hemos chocado
		SlideAlongSurface(Adjusted, 1.f - Hit.Time, Hit.Normal, Hit, true);
	}

	//Si después de realizar el movimiento, no cumplimos las condiciones, salimos del slide
	FHitResult NewSurfaceHit;
	if (!GetSlideSurface(NewSurfaceHit) || Velocity.SizeSquared() < pow(Min_SlideSpeed, 2))
	{
		SetMovementMode(MOVE_Walking);
	}

	//Terminado el movimiento, actualizamos la velocidad del personaje
	if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime; // v = dx / dt
	}

}

bool UDustyCharacterMovementComponent::GetSlideSurface(FHitResult& Hit) const
{
	// El componente que se actualiza, aka el root de base
	FVector Start = UpdatedComponent->GetComponentLocation();
	FVector End = Start + DustyCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 2.f * FVector::DownVector;
	return GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, DustyCharacterOwner->GetIgnoreCharacterCollisionParams());

}

void UDustyCharacterMovementComponent::EnterProne(EMovementMode PrevMovementMode, ECustomMovementMode PrevCustomMovementMode)
{
	bWantsToCrouch = true;
	FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, true, NULL);

	//Animation purposes
	bProning = true;
	bCrouching = false;

	DustyCharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(DustyCharacterOwner->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2);
}

void UDustyCharacterMovementComponent::ExitProne()
{
	//Animation purposes
	bProning = false;	


	DustyCharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(DustyCharacterOwner->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());

	
}

void UDustyCharacterMovementComponent::PhysProne(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}


	if (!CharacterOwner || (!CharacterOwner->Controller && !bRunPhysicsWithNoController && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && (CharacterOwner->GetLocalRole() != ROLE_SimulatedProxy)))
	{
		Acceleration = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		return;
	}
	
	bJustTeleported = false;
	bool bCheckedFall = false;
	bool bTriedLedgeMove = false;
	float remainingTime = deltaTime;

	//Substepping
	// Perform the move
	while ((remainingTime >= MIN_TICK_TIME) && (Iterations < MaxSimulationIterations) && CharacterOwner && (CharacterOwner->Controller || bRunPhysicsWithNoController || (CharacterOwner->GetLocalRole() == ROLE_SimulatedProxy)))
	{
		Iterations++;
		bJustTeleported = false;
		const float timeTick = GetSimulationTimeStep(remainingTime, Iterations);
		remainingTime -= timeTick;

		// Save current values in case substep fails and need to revert
		UPrimitiveComponent* const OldBase = GetMovementBase();
		const FVector PreviousBaseLocation = (OldBase != NULL) ? OldBase->GetComponentLocation() : FVector::ZeroVector;
		const FVector OldLocation = UpdatedComponent->GetComponentLocation();
		const FFindFloorResult OldFloor = CurrentFloor;

		// Ensure velocity is horizontal.
		MaintainHorizontalGroundVelocity();
		const FVector OldVelocity = Velocity;
		Acceleration.Z = 0.f;

		// Apply acceleration
		CalcVelocity(timeTick, GroundFriction, false, GetMaxBrakingDeceleration());

		// Compute move parameters
		const FVector MoveVelocity = Velocity;
		const FVector Delta = timeTick * MoveVelocity; // dx = v * dt
		const bool bZeroDelta = Delta.IsNearlyZero();
		FStepDownResult StepDownResult;

		if (bZeroDelta)
		{
			remainingTime = 0.f;
		}
		else
		{
			MoveAlongFloor(MoveVelocity, timeTick, &StepDownResult);

			if (IsFalling())
			{
				// pawn decided to jump up
				const float DesiredDist = Delta.Size();
				if (DesiredDist > KINDA_SMALL_NUMBER)
				{
					const float ActualDist = (UpdatedComponent->GetComponentLocation() - OldLocation).Size2D();
					remainingTime += timeTick * (1.f - FMath::Min(1.f, ActualDist / DesiredDist));
				}
				StartNewPhysics(remainingTime, Iterations);
				return;
			}
		}

		// Update floor.
		// StepUp might have already done it for us.
		if (StepDownResult.bComputedFloor)
		{
			CurrentFloor = StepDownResult.FloorResult;
		}
		else
		{
			FindFloor(UpdatedComponent->GetComponentLocation(), CurrentFloor, bZeroDelta, NULL);
		}

		/////////////////////////////////////// MUCHA BULLSHIT PARA LOS BORDES //////////////////////////////////////////////////////////////
		// 
		// check for ledges here
		const bool bCheckLedges = !CanWalkOffLedges();
		if (bCheckLedges && !CurrentFloor.IsWalkableFloor())
		{
			// calculate possible alternate movement
			const FVector GravDir = FVector(0.f, 0.f, -1.f);
			const FVector NewDelta = bTriedLedgeMove ? FVector::ZeroVector : GetLedgeMove(OldLocation, Delta, GravDir);
			if (!NewDelta.IsZero())
			{
				// first revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, false);

				// avoid repeated ledge moves if the first one fails
				bTriedLedgeMove = true;

				// Try new movement direction
				Velocity = NewDelta / timeTick; // v = dx/dt
				remainingTime += timeTick;
				continue;
			}
			else
			{
				// see if it is OK to jump
				// @todo collision : only thing that can be problem is that oldbase has world collision on
				bool bMustJump = bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;

				// revert this move
				RevertMove(OldLocation, OldBase, PreviousBaseLocation, OldFloor, true);
				remainingTime = 0.f;
				break;
			}
		}
		else
		{
			// Validate the floor check
			if (CurrentFloor.IsWalkableFloor())
			{
				AdjustFloorHeight();
				SetBase(CurrentFloor.HitResult.Component.Get(), CurrentFloor.HitResult.BoneName);
			}
			else if (CurrentFloor.HitResult.bStartPenetrating && remainingTime <= 0.f)
			{
				// The floor check failed because it started in penetration
				// We do not want to try to move downward because the downward sweep failed, rather we'd like to try to pop out of the floor.
				FHitResult Hit(CurrentFloor.HitResult);
				Hit.TraceEnd = Hit.TraceStart + FVector(0.f, 0.f, MAX_FLOOR_DIST);
				const FVector RequestedAdjustment = GetPenetrationAdjustment(Hit);
				ResolvePenetration(RequestedAdjustment, Hit, UpdatedComponent->GetComponentQuat());
				bForceNextFloorCheck = true;
			}

			// check if just entered water
			if (IsSwimming())
			{
				StartSwimming(OldLocation, Velocity, timeTick, remainingTime, Iterations);
				return;
			}

			// See if we need to start falling.
			if (!CurrentFloor.IsWalkableFloor() && !CurrentFloor.HitResult.bStartPenetrating)
			{
				const bool bMustJump = bJustTeleported || bZeroDelta || (OldBase == NULL || (!OldBase->IsQueryCollisionEnabled() && MovementBaseUtility::IsDynamicBase(OldBase)));
				if ((bMustJump || !bCheckedFall) && CheckFall(OldFloor, CurrentFloor.HitResult, Delta, OldLocation, remainingTime, timeTick, Iterations, bMustJump))
				{
					return;
				}
				bCheckedFall = true;
			}
		}
		/////////////////////////////////////////// DEMASIADA BULLSHIT ////////////////////////////////////////////////////


		// Allow overlap events and such to change physics state and velocity
		if (IsMovingOnGround())
		{
			// Make velocity reflect actual move
			if (!bJustTeleported && !HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity() && timeTick >= MIN_TICK_TIME)
			{
				// TODO-RootMotionSource: Allow this to happen during partial override Velocity, but only set allowed axes?
				Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / timeTick; // v = dx / dt
				MaintainHorizontalGroundVelocity();
			}
		}

		// If we didn't move at all this iteration then abort (since future iterations will also be stuck).
		if (UpdatedComponent->GetComponentLocation() == OldLocation)
		{
			remainingTime = 0.f;
			break;
		}
	}

	if (IsMovingOnGround())
	{
		MaintainHorizontalGroundVelocity();
	}
}

bool UDustyCharacterMovementComponent::CanProne() const
{
	return IsMovementMode(MOVE_Walking) && IsCrouching();

}
