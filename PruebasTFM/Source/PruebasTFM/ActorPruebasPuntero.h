// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPruebasPuntero.generated.h"

class UMyGameInstanceSubsystem;

UCLASS()
class PRUEBASTFM_API AActorPruebasPuntero : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorPruebasPuntero();

	UMyGameInstanceSubsystem* SubsystemInstance;
	FTimerManager TimerManager;

	FTimerHandle TimeHandle;
	FTimerHandle CancelTimerHandle;

	void PrintScore();

	void CancelTimeHandle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
