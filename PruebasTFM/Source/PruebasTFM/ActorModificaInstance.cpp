// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorModificaInstance.h"
#include "MyGameInstanceSubsystem.h"

// Sets default values
AActorModificaInstance::AActorModificaInstance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AActorModificaInstance::BeginPlay()
{
	Super::BeginPlay();
	GetGameInstance()->GetTimerManager().SetTimer
	(
		TimeHandle,
		this,
		&AActorModificaInstance::AddScore,
		5,
		true
	);
}

// Called every frame
void AActorModificaInstance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void AActorModificaInstance::AddScore()
{
	GetGameInstance()->GetSubsystem<UMyGameInstanceSubsystem>()->index++;
}

