// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DustyCharacterMovementComponent.generated.h"


class ADustyCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None	 UMETA(Hidden),
	CMOVE_Slide	 UMETA(DisplayName = "Slide"),
	CMOVE_MAX	 UMETA(Hidden)
};


UCLASS()
class PRUEBASTFM_API UDustyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	

public:
	UDustyCharacterMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetSprinting(bool NewSprinting);


	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetCrouched();


	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsCustomMovementMode(ECustomMovementMode Mode) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Max_SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Max_WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Min_SlideSpeed = 350;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SlideEnterImpulse = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SlideGravityForce = 5000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SlideFriction = 1.3;

protected:
	bool bIsSprinting = false;
	bool bPrevWantsToCrouch = false;

	UPROPERTY(Transient)
	ADustyCharacter* DustyCharacterOwner;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void InitializeComponent() override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

public:

	virtual bool IsMovingOnGround() const override;

	virtual bool CanCrouchInCurrentState() const override;


private:

	void EnterSlide();
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
};
