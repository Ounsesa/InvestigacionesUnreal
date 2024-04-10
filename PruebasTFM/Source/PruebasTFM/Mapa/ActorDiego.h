// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorDiego.generated.h"

class UInteractiveComponent;

UCLASS()
class PRUEBASTFM_API AActorDiego : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorDiego();

	UPROPERTY(EditAnywhere)
	UInteractiveComponent* InteractiveComponent;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	void FunctionForErik(AActor* OtherActor);

};
