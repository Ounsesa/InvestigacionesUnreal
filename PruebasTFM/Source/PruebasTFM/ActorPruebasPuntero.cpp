// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPruebasPuntero.h"
#include "MyGameInstanceSubsystem.h"

// Sets default values
AActorPruebasPuntero::AActorPruebasPuntero()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AActorPruebasPuntero::PrintScore()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Index: %i"), SubsystemInstance->index));
}

void AActorPruebasPuntero::CancelTimeHandle()
{
	GetGameInstance()->GetTimerManager().ClearTimer(TimeHandle);
}

// Called when the game starts or when spawned
void AActorPruebasPuntero::BeginPlay()
{
	Super::BeginPlay();
	
	SubsystemInstance = GetGameInstance()->GetSubsystem<UMyGameInstanceSubsystem>();

	if (SubsystemInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Pues se crea y se enlaza solo solito"));
	}


	GetWorld()->GetTimerManager().SetTimer
	(
		TimeHandle,
		this,
		&AActorPruebasPuntero::PrintScore,
		5,
		true
	);
	GetWorld()->GetTimerManager().SetTimer
	(
		CancelTimerHandle,
		this,
		&AActorPruebasPuntero::CancelTimeHandle,
		15,
		false
	);
}

// Called every frame
void AActorPruebasPuntero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

