// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/CancellableAsyncAction.h"
#include "MyCancellableAsyncAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMyDelayAsyncActionEvent);
/**
 * 
 */
UCLASS()
class PRUEBASTFM_API UMyCancellableAsyncAction : public UCancellableAsyncAction
{
	GENERATED_BODY()
public:
    /**
     * Start a delay.
     * @param DelayTime		How long the delay will last.
     * @param WorldContext	Object from which the world will be derived.
     */


    //No sé muy bien cómo, pero el meta este mete el valor al parametro WorldContext. No he visto otro lado donde se meta
    UFUNCTION(BlueprintCallable, DisplayName = "Delay (Custom)", meta = (WorldContext = "WorldContext"))
        static UMyCancellableAsyncAction* MyDelayAsyncAction(const UObject* WorldContext, float DelayTime);

    /** A delegate called when the async action completes. */
    UPROPERTY(BlueprintAssignable)
        FMyDelayAsyncActionEvent OnComplete;

    /** A delegate called when the async action fails. */
    UPROPERTY(BlueprintAssignable)
        FMyDelayAsyncActionEvent OnFail;

    // Start UCancellableAsyncAction Functions
    virtual void Activate() override;
    virtual void Cancel() override;
    // End UCancellableAsyncAction Functions

    // Start UObject Functions
    virtual UWorld* GetWorld() const override
    {
        return ContextWorld.IsValid() ? ContextWorld.Get() : nullptr;
    }
    // End UObject Functions

private:
    /** The context world of this action. */
    TWeakObjectPtr<UWorld> ContextWorld = nullptr;

    /** The time this action will wait before finishing. */
    float DelayTime = 0.f;

    /** The timer handle. */

    FTimerHandle StartDelay;

    void onComplete();

};
