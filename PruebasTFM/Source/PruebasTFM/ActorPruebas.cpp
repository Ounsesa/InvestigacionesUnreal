// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPruebas.h"
#include "MyGameInstanceSubsystem.h"
#include "MyDataAsset.h"



// Sets default values
AActorPruebas::AActorPruebas()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AActorPruebas::PrintScore()
{
	int index = GetGameInstance()->GetSubsystem<UMyGameInstanceSubsystem>()->index; 
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Index: %i"), index));
}

// Called when the game starts or when spawned
void AActorPruebas::BeginPlay()
{
	Super::BeginPlay();

	UMyGameInstanceSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UMyGameInstanceSubsystem>();

	if (Subsystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Pues se crea y se enlaza solo solito"));
	}

	GetGameInstance()->GetTimerManager().SetTimer
	(
		TimeHandle,
		this,
		&AActorPruebas::PrintScore,
		3,
		true
	);

	if (DataAssetInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Health: %d"), DataAssetInstance->index));
	}
}

// Called every frame
void AActorPruebas::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

