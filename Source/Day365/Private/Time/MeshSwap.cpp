#include "Time/MeshSwap.h"
#include "Components/StaticMeshComponent.h"

AMeshSwap::AMeshSwap()
{
    PrimaryActorTick.bCanEverTick = false;

    PresentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PresentMesh"));
    RootComponent = PresentMesh;

    PastMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PastMesh"));
    PastMesh->SetupAttachment(RootComponent);

    FutureMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FutureMesh"));
    FutureMesh->SetupAttachment(RootComponent);
}

void AMeshSwap::BeginPlay()
{
    Super::BeginPlay();

    PastMesh->SetVisibility(false);
    PresentMesh->SetVisibility(true);
    FutureMesh->SetVisibility(false);
}

void AMeshSwap::OnTimeChanged_Implementation(const FName &NewTimeState)
{
    CurrentTimeState = NewTimeState;

    // PastMesh->SetVisibility(NewState == ETimeState::Past);
    // PresentMesh->SetVisibility(NewState == ETimeState::Present);
    // FutureMesh->SetVisibility(NewState == ETimeState::Future);
}