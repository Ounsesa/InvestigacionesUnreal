// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorDiego.h"
#include "ActorErik.h"
#include "InteractiveComponent.h"
#include "HubActor.h"
#include "Components/PointLightComponent.h"

// Sets default values
AActorErik::AActorErik()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>("Interactive");

	light = CreateDefaultSubobject<UPointLightComponent>("Light");
	light->SetupAttachment(RootComponent);
	light->SetLightColor(FLinearColor::Red);
	light->SetVisibility(false);
}

// Called when the game starts or when spawned
void AActorErik::BeginPlay()
{
	Super::BeginPlay();


	InteractiveComponent->MapObjectFunction.Add(InteractiveComponent->InteractiveObjects[0]);
	InteractiveComponent->MapObjectFunction[InteractiveComponent->InteractiveObjects[0]].BindUObject(this, &AActorErik::FunctionForDiego);

	InteractiveComponent->MapObjectFunction.Add(InteractiveComponent->InteractiveObjects[1]);
	InteractiveComponent->MapObjectFunction[InteractiveComponent->InteractiveObjects[1]].BindUObject(this, &AActorErik::FunctionForHub);
	
}

// Called every frame
void AActorErik::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AActorErik::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AActorDiego* DiegoActor = Cast<AActorDiego>(OtherActor);
	if (DiegoActor)
	{
		DiegoActor->InteractiveComponent->ReceiveMessage(this);
	}

	AHubActor* HubActor = Cast<AHubActor>(OtherActor);
	if (HubActor)
	{
		HubActor->InteractiveComponent->ReceiveMessage(this);
	}
}

void AActorErik::FunctionForDiego(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Black, OtherActor->GetClass()->GetName());
}

void AActorErik::FunctionForHub(AActor* OtherActor)
{
	AHubActor* HubActor = Cast<AHubActor>(OtherActor);
	if (HubActor)
	{
		Lighted = HubActor->lighted;
		light->SetVisibility(Lighted);
	}
}
