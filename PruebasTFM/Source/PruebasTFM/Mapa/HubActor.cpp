// Fill out your copyright notice in the Description page of Project Settings.


#include "HubActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"
#include "InteractiveComponent.h"
#include "ActorErik.h"

// Sets default values
AHubActor::AHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	light = CreateDefaultSubobject<UPointLightComponent>("Light");
	light->SetupAttachment(RootComponent);
	light->SetLightColor(FLinearColor::Blue);


	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>("Interactive");
}

// Called when the game starts or when spawned
void AHubActor::BeginPlay()
{
	Super::BeginPlay();


	InteractiveComponent->MapObjectFunction.Add(InteractiveComponent->InteractiveObjects[0]);
	InteractiveComponent->MapObjectFunction[InteractiveComponent->InteractiveObjects[0]].BindUObject(this, &AHubActor::ayuwokiErik);
}

// Called every frame
void AHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHubActor::ayuwokiErik(AActor* OtherActor)
{
	AActorErik* ErikActor = Cast<AActorErik>(OtherActor);
	if (ErikActor)
	{
		bool newLight = ErikActor->Lighted;
		ErikActor->InteractiveComponent->ReceiveMessage(this);
		lighted = newLight;
		light->SetVisibility(lighted);
	}
}

void AHubActor::ayuwokiDiego()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Funciona Diego"));
}

void AHubActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	/*AActorErik* ErikActor = Cast<AActorErik>(OtherActor);
	if (ErikActor)
	{
		ErikActor->InteractiveComponent->ReceiveMessage(this);
	}*/
}
