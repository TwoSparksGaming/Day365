#include "NPCBase.h"
#include "Animation/AnimSingleNodeInstance.h"
#include "Time/TimeManager.h"
#include "Kismet/GameplayStatics.h"

ANPCBase::ANPCBase()
{
    PrimaryActorTick.bCanEverTick = false;
}

void ANPCBase::BeginPlay()
{
    Super::BeginPlay();

    if (TimeManager == nullptr)
    {
        TArray<AActor *> TimeManagers;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeManager::StaticClass(), TimeManagers);

        if (TimeManagers.IsEmpty() == false)
        {
            TimeManager = Cast<ATimeManager>(TimeManagers[0]);
        }
    }

    if (TimeManager == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ANPCBase: TimeManager not found"));
        return;
    }

    FName CurrentState = TimeManager->GetCurrentTimeState();

    for (const FNPCTimeData &Data : TimeDataList)
    {
        if (Data.TimeState == CurrentState)
        {
            ApplyNPCData(Data);
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("ANPCBase: No data found for initial state %s"), *CurrentState.ToString());
}

void ANPCBase::OnTimeChanged_Implementation(const FName &NewState)
{
    for (const FNPCTimeData &Data : TimeDataList)
    {
        if (Data.TimeState == NewState)
        {
            ApplyNPCData(Data);
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("ANPCBase: No data found for state %s"), *NewState.ToString());
}

void ANPCBase::ApplyNPCData(const FNPCTimeData &Data)
{
    SetActorLocation(Data.Location);
    SetActorRotation(Data.Rotation);

    if (Data.Pose == nullptr)
        return;

    USkeletalMeshComponent *SkelMesh = GetMesh();
    if (SkelMesh == nullptr)
        return;

    SkelMesh->PlayAnimation(Data.Pose, false);
}