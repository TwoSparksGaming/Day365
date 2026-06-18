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
    if (!PlayerCharacter)
        return;

    AMainCharacter *MainCharacter = Cast<AMainCharacter>(PlayerCharacter);
    if (!MainCharacter)
        return;

    UE_LOG(LogTemp, Warning, TEXT("AInteractableBase::Interact_Implementation()"));

    // ADay365Character로 캐스트는 나중에 C++ 캐릭터 클래스 만들면 추가
    if (ItemData.InteractionType == EInteractionType::Acquire)
    {
        if (ItemData.ItemType == EItemType::Watch)
        {
            MainCharacter->AquirePocketWatch();
            return;
        }
        else
        {
            MainCharacter->AddItemToInventory(ItemData);
        }
    }
    else if (ItemData.InteractionType == EInteractionType::Puzzle)
    {
    }
    else // EInteractionType::NoInteraction
    {
    }

    switch (ItemData.ItemType)
    {
    case EItemType::Watch:
    case EItemType::Tool:
    case EItemType::Note:
    case EItemType::Special:
        OnAcquire_Implementation(nullptr);
        break;
    default:
        break;
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