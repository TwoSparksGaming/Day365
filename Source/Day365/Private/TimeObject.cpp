#include "TimeObject.h"
#include "Components/StaticMeshComponent.h"

ATimeObject::ATimeObject()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ATimeObject::BeginPlay()
{
    Super::BeginPlay();

    if (TimeStateDataList.IsEmpty() == false)
    {
        ApplyTimeStateData(TimeStateDataList[0]);
    }
}

void ATimeObject::OnTimeChanged_Implementation(const FName &NewTimeState)
{
    for (const FTimeStateData &Data : TimeStateDataList)
    {
        if (Data.TimeState == NewTimeState)
        {
            ApplyTimeStateData(Data);
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("ATimeObject: No data found for state %s"), *NewTimeState.ToString());
}

void ATimeObject::ApplyTimeStateData(const FTimeStateData &Data)
{
    if (Data.Mesh != nullptr)
    {
        MeshComponent->SetStaticMesh(Data.Mesh);
    }

    for (int32 i = 0; i < Data.Materials.Num(); i++)
    {
        if (Data.Materials[i] != nullptr)
        {
            MeshComponent->SetMaterial(i, Data.Materials[i]);
        }
    }
}