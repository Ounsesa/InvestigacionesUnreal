// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "DustyPlayerCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class PRUEBASTFM_API ADustyPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Camera")
	float CrouchBlendDuration = 0.5f;

	float CrouchBlendTime = 0.0f;

public:

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;


};
