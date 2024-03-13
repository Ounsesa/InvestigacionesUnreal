// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyCharacterMovementComponent.h"

void UDustyCharacterMovementComponent::SetSprinting(bool NewSprinting)
{
	bIsSprinting = NewSprinting;
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

}