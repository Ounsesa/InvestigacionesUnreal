// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HubActor.generated.h"

class UPointLightComponent;
class UInteractiveComponent;

UCLASS()
class PRUEBASTFM_API AHubActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHubActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magnetism, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* light;


	UPROPERTY(EditAnywhere)
	UInteractiveComponent* InteractiveComponent;


	
	bool lighted = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	void ayuwokiErik(AActor* OtherActor);
	void ayuwokiDiego();

};
