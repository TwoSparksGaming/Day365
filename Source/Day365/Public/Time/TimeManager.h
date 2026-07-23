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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName CurrentTimeState = "t1";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName MovingTimeState = "t1";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    UDataTable *TimeTransitionTable = nullptr;

  public:
    UFUNCTION(BlueprintPure, Category = "Time")
    FName GetCurrentTimeState() const
    {
        return CurrentTimeState;
    }

    UFUNCTION(BlueprintPure, Category = "Time")
    FName GetMovingTimeState() const
    {
        return MovingTimeState;
    }

    UFUNCTION(BlueprintCallable, Category = "Time")
    void NotifyTimeChanged(FName NewState);

    UFUNCTION(BlueprintCallable, Category = "Time")
    bool CanMoveForward();

    UFUNCTION(BlueprintCallable, Category = "Time")
    bool CanMoveBackward();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void ChangeTime();

  private:
    const FTimeTransition *FindTransition() const;
    bool IsMovingBackward = false;
};