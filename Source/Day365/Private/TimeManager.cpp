#include "TimeManager.h"
#include "TimeAwareInterface.h"
#include "Kismet/GameplayStatics.h"

ATimeManager::ATimeManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATimeManager::NotifyTimeChanged(ETimeState NewState)
{
    CurrentState = NewState;

    TArray<AActor *> TimeAwareActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTimeAwareInterface::StaticClass(), TimeAwareActors);

    for (AActor *Actor : TimeAwareActors)
    {
        if (Actor == nullptr)
            continue;

        ITimeAwareInterface::Execute_OnTimeChanged(Actor, NewState);
    }
}