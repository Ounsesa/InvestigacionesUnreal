// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorErik.generated.h"

class UInteractiveComponent;
class UPointLightComponent;

UCLASS()
class PRUEBASTFM_API AActorErik : public AActor
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnetism, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* light;

public:	
	// Sets default values for this actor's properties
	AActorErik();

	UPROPERTY(EditAnywhere)
	UInteractiveComponent* InteractiveComponent;


	UPROPERTY(EditAnywhere)
	bool Lighted = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


	void FunctionForDiego(AActor* OtherActor);
	void FunctionForHub(AActor* OtherActor);
};
