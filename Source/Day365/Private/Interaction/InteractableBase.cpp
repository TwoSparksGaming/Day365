#include "Interaction/InteractableBase.h"
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
    else if (ItemData.InteractionType == EInteractionType::ZoomInPuzzle)
    {
        UCameraComponent *InteractCamera = GetInteractCamera();
        if (InteractCamera == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("InteractCamera is not set"));
            return;
        }

        MainCharacter->SwitchCamera(InteractCamera, 1.f, true);
        MainCharacter->SetCurrentInteractObject(this);
    }
    else // EInteractionType::NoInteraction
    {
    }
}

bool AInteractableBase::CanInteract_Implementation()
{
    return bCanInteract;
}
