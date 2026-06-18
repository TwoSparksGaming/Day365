#include "PlaceSpot.h"
#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"

APlaceSpot::APlaceSpot()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void APlaceSpot::BeginPlay()
{
    Super::BeginPlay();
}

void APlaceSpot::Interact_Implementation()
{
    if (bHasItem == true)
    {
        PickUpItem();
    }
    else
    {
        PlaceItem();
    }
}

bool APlaceSpot::CanInteract_Implementation()
{
    return bHasItem;
}

bool APlaceSpot::IsAcceptable(FName ItemID) const
{
    return AcceptableItems.Contains(ItemID);
}

void APlaceSpot::PlaceItem()
{
    if (bHasItem == true)
        return;

    if (ItemVisualDataTable == nullptr)
        return;

    AMainCharacter *MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (MainCharacter == nullptr)
        return;

    FItemData NewItem = MainCharacter->GetSelectedItem();
    if (IsAcceptable(NewItem.ItemID) == false)
        return;

    FItemVisualData *VisualData = ItemVisualDataTable->FindRow<FItemVisualData>(NewItem.ItemID, TEXT("PlaceItem"));
    if (VisualData == nullptr)
        return;

    // 메시
    MeshComponent->SetStaticMesh(VisualData->Mesh);

    // 머티리얼
    for (int32 i = 0; i < VisualData->Materials.Num(); i++)
    {
        MeshComponent->SetMaterial(i, VisualData->Materials[i]);
    }

    // Visibility
    MeshComponent->SetVisibility(true);

    PlacedItem = NewItem;
    bHasItem = true;

    // OnToggleItem(NewItem, true);
}

void APlaceSpot::PickUpItem()
{
    if (bHasItem == false)
        return;

    AMainCharacter *MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (MainCharacter == nullptr)
        return;

    MainCharacter->AddItemToInventory(PlacedItem);

    PlacedItem = FItemData();
    bHasItem = false;

    // OnToggleItem({}, false);
}