// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DustyCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PRUEBASTFM_API UDustyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetSprinting(bool NewSprinting);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Max_SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Max_WalkSpeed;

protected:
	bool bIsSprinting;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
};
