#include "InteractableBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"

AInteractableBase::AInteractableBase()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void AInteractableBase::BeginPlay()
{
    Super::BeginPlay();
}

void AInteractableBase::Interact_Implementation()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerCharacter == nullptr)
        return;

    AMainCharacter *MainCharacter = Cast<AMainCharacter>(PlayerCharacter);
    if (MainCharacter == nullptr)
        return;

    UE_LOG(LogTemp, Warning, TEXT("AInteractableBase::Interact_Implementation()"));

    if (ItemData.InteractionType == EInteractionType::Acquire)
    {
        if (ItemData.ItemType == EItemType::Watch)
        {
            MainCharacter->AquirePocketWatch();
        }
        else
        {
            MainCharacter->AddItemToInventory(ItemData);
        }

        Destroy();
    }
    else if (ItemData.InteractionType == EInteractionType::Puzzle)
    {
    }
    else // EInteractionType::NoInteraction
    {
    }
}

bool AInteractableBase::CanInteract_Implementation()
{
    return bCanInteract;
}

void AInteractableBase::OnAcquire_Implementation(ACharacter *PlayerCharacter)
{
    // 블루프린트에서 Override
}

void AInteractableBase::OnPuzzleInteract_Implementation(ACharacter *PlayerCharacter)
{
    // 블루프린트에서 Override
}