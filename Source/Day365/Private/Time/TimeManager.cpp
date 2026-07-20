#include "Time/TimeManager.h"
#include "Time/TimeAwareInterface.h"
#include "Kismet/GameplayStatics.h"

ATimeManager::ATimeManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ATimeManager::NotifyTimeChanged(FName NewTimeState)
{
    CurrentTimeState = NewTimeState;

    // TimeAwareInterface
    TArray<AActor *> TimeAwareActors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTimeAwareInterface::StaticClass(), TimeAwareActors);

    for (AActor *Actor : TimeAwareActors)
    {
        if (Actor == nullptr)
            continue;

        ITimeAwareInterface::Execute_OnTimeChanged(Actor, NewTimeState);
    }

    // PuzzleManager
    TArray<AActor *> PuzzleManagers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APuzzleManager::StaticClass(), PuzzleManagers);

    for (AActor *Actor : PuzzleManagers)
    {
        APuzzleManager *PuzzleManager = Cast<APuzzleManager>(Actor);
        if (PuzzleManager == nullptr)
            continue;

        PuzzleManager->CheckPuzzle();
    }
}

bool ATimeManager::CanMoveForward()
{
    const FTimeTransition *Transition = FindTransition();
    if (Transition == nullptr)
        return false;

    MovingTimeState = Transition->Forward;
    return MovingTimeState != NAME_None;
}

bool ATimeManager::CanMoveBackward()
{
    const FTimeTransition *Transition = FindTransition();
    if (Transition == nullptr)
        return false;

    MovingTimeState = Transition->Backward;
    return MovingTimeState != NAME_None;
}

void ATimeManager::ChangeTime()
{
    // 나중에 시간 바뀔때 레벨 자체가 바뀐다거나, 조건에 따라 t2_right | t2_wrong 처럼 분기할때 필요할듯 해서 남겨둠.
    /*
    if (Transition->Forward.PuzzleManager != nullptr)
    {
        bool bCorrect = Transition->Forward.PuzzleManager->IsCorrect();
        NextState = bCorrect ? Transition->Forward.CorrectTimeState : Transition->Forward.WrongTimeState;
    }
    */

    NotifyTimeChanged(MovingTimeState);
}

const FTimeTransition *ATimeManager::FindTransition() const
{
    if (TimeTransitionTable == nullptr)
        return nullptr;

    TArray<FTimeTransition *> AllRows;
    TimeTransitionTable->GetAllRows<FTimeTransition>(TEXT("ATimeManager::FindTransition"), AllRows);

    for (FTimeTransition *Transition : AllRows)
    {
        if (Transition->Current == CurrentTimeState)
            return Transition;
    }

    return nullptr;
}