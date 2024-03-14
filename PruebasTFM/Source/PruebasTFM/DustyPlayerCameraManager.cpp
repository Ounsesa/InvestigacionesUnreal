// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyPlayerCameraManager.h"
#include "DustyCharacter.h"
#include "DustyCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

void ADustyPlayerCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	if (ADustyCharacter* DustyCharacter = Cast<ADustyCharacter>(GetOwningPlayerController()->GetPawn()))
	{
		UDustyCharacterMovementComponent* DustyCharacterMovementComponent = DustyCharacter->GetDustyMovementComponent();
		FVector TargetCrouchOffset = FVector(0,0, DustyCharacterMovementComponent->GetCrouchedHalfHeight() - DustyCharacter->GetClass()->GetDefaultObject<ACharacter>()->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight());
	
		FVector Offset = FMath::Lerp(FVector::ZeroVector, TargetCrouchOffset, FMath::Clamp(CrouchBlendTime / CrouchBlendDuration, 0.f, 1.f));

		if (DustyCharacterMovementComponent->IsCrouching())
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime + DeltaTime, 0.f, CrouchBlendDuration);
			Offset -= TargetCrouchOffset;
		}
		else
		{
			CrouchBlendTime = FMath::Clamp(CrouchBlendTime - DeltaTime, 0.f, CrouchBlendDuration);
		}

		if (DustyCharacterMovementComponent->IsMovingOnGround())
		{
			OutVT.POV.Location += Offset;
		}

	}
}
