#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes.h"
#include "TimeManager.generated.h"

UCLASS()
class DAY365_API ATimeManager : public AActor
{
    GENERATED_BODY()

  public:
    ATimeManager();

  protected:
    UPROPERTY(BlueprintReadOnly, Category = "Time")
    ETimeState CurrentState = ETimeState::Present;

  public:
    UFUNCTION(BlueprintCallable, Category = "Time")
    void NotifyTimeChanged(ETimeState NewState);

    UFUNCTION(BlueprintPure, Category = "Time")
    ETimeState GetCurrentState() const
    {
        return CurrentState;
    }
};