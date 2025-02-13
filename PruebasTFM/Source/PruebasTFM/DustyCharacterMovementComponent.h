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
	CMOVE_Prone  UMETA(DisplayName = "Prone"),
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
	void CrouchPressed();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CrouchReleased();


	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsCustomMovementMode(ECustomMovementMode Mode) const;
	UFUNCTION(BlueprintPure) 
	bool IsMovementMode(EMovementMode InMovementMode) const;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BrakingDecelerationSliding = 1000.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ProneMaxSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ProneHoldDuration = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ProneSlideEnterImpulse = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BrakingDecelerationProne = 2500.f;

protected:
	bool bIsSprinting = false;
	bool bPrevWantsToCrouch = false;
	bool bPrevWantsToProne = false;

	UPROPERTY(Transient)
	ADustyCharacter* DustyCharacterOwner;

	FTimerHandle ProneTimerHandle;


	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual void InitializeComponent() override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

public:

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;


	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bSliding = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bProning = false;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bCrouching = false;


	UFUNCTION(BlueprintCallable, Category = "Movement")
	void EnterSlide();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CustomProne();

private:
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;

private:
	void TryEnterProne() { bPrevWantsToProne = true; };

	void EnterProne(EMovementMode PrevMovementMode, ECustomMovementMode PrevCustomMovementMode);
	void ExitProne();
	void PhysProne(float deltaTime, int32 Iterations);
	bool CanProne() const;

	
	
};
