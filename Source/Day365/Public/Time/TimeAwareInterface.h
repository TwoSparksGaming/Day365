#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameTypes.h"
#include "TimeAwareInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UTimeAwareInterface : public UInterface
{
    GENERATED_BODY()
};

class DAY365_API ITimeAwareInterface
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Time")
    void OnTimeChanged(ETimeState NewState);
    virtual void OnTimeChanged_Implementation(ETimeState NewState) {}
};