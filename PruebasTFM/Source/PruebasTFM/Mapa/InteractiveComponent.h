// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"


DECLARE_DELEGATE_OneParam(FDelegateName, AActor*)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRUEBASTFM_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveComponent();

	//Actors that can interact with Owner
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AActor>> InteractiveObjects;

	//Added in Owner
	TMap<TSubclassOf<AActor>, FDelegateName> MapObjectFunction;

	void ReceiveMessage(AActor* OtherActor);


		
};
