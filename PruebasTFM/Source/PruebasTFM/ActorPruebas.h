// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPruebas.generated.h"

class UMyDataAsset;

UCLASS()
class PRUEBASTFM_API AActorPruebas : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorPruebas();

	FTimerHandle TimeHandle;

	void PrintScore();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMyDataAsset* DataAssetInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
