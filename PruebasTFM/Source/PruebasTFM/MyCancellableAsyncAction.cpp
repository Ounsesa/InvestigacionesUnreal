// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCancellableAsyncAction.h"
#include "Engine.h"

UMyCancellableAsyncAction* UMyCancellableAsyncAction::MyDelayAsyncAction(const UObject* WorldContext, float DelayTime)
{
    // This function is just a factory that creates a UMyDelayAsyncAction instance.

    // We must have a valid contextual world for this action, so we don't even make it
    // unless we can resolve the UWorld from WorldContext.
    UWorld* ContextWorld = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::ReturnNull);

    // Esto es debug mio
    UWorld* World = GEngine->GetWorld();
    if (!World)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, TEXT("Por alguna razón no puedo sacar el world del gEngine"));
    }


    // Create a new UMyDelayAsyncAction, and store function arguments in it.
    UMyCancellableAsyncAction* NewAction = NewObject<UMyCancellableAsyncAction>();
    NewAction->ContextWorld = ContextWorld;
    NewAction->DelayTime = DelayTime;
    NewAction->RegisterWithGameInstance(ContextWorld->GetGameInstance());
    return NewAction;
}

void UMyCancellableAsyncAction::Activate()
{
    // When the async action is ready to activate, set a timer using the world's FTimerManager.
    if (const UWorld* World = GetWorld())
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("A"));
        // The timer manager is a singleton, and GetTimerManger() accessor will always return a valid one.
        FTimerManager& TimerManager = World->GetTimerManager();
        TimerManager.SetTimer(StartDelay, this, &UMyCancellableAsyncAction::onComplete, DelayTime, false);                    
        return;
    }

    // If something failed, we can broadcast OnFail, and then wrap up.
    OnFail.Broadcast();
    Cancel();
}

void UMyCancellableAsyncAction::Cancel()
{
    Super::Cancel();

    // Cancel the timer if it's ongoing, so OnComplete never broadcasts.
    if (StartDelay.IsValid())
    {
        if (const UWorld* World = GetWorld())
        {
            FTimerManager& TimerManager = World->GetTimerManager();
            TimerManager.ClearTimer(StartDelay);
        }
    }
}

void UMyCancellableAsyncAction::onComplete()
{
    OnComplete.Broadcast();
    Cancel();
}

