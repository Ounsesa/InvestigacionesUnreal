// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorDiego.h"
#include "ActorErik.h"
#include "InteractiveComponent.h"

// Sets default values
AActorDiego::AActorDiego()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>("Interactive");

}

// Called when the game starts or when spawned
void AActorDiego::BeginPlay()
{
	Super::BeginPlay();
	
	InteractiveComponent->MapObjectFunction.Add(InteractiveComponent->InteractiveObjects[0]);
	InteractiveComponent->MapObjectFunction[InteractiveComponent->InteractiveObjects[0]].BindUObject(this, &AActorDiego::FunctionForErik);

}



// Called every frame
void AActorDiego::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActorDiego::NotifyActorBeginOverlap(AActor* OtherActor)
{
	AActorErik* ErikActor = Cast<AActorErik>(OtherActor);
	if (ErikActor)
	{
		ErikActor->InteractiveComponent->ReceiveMessage(this);
	}
}

void AActorDiego::FunctionForErik(AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Black, OtherActor->GetClass()->GetName());
}

