#include "Interaction/PlaceSpot.h"
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

// Func
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

    FItemData SelectedItem = MainCharacter->GetSelectedItem();
    if (IsAcceptable(SelectedItem.ItemID) == false)
        return;

    FItemVisualData *VisualData =
        ItemVisualDataTable->FindRow<FItemVisualData>(SelectedItem.ItemID, TEXT("APlaceSpot::PlaceItem()"));
    if (VisualData == nullptr)
        return;

    // �޽�
    MeshComponent->SetStaticMesh(VisualData->Mesh);

    // ��Ƽ����
    for (int32 i = 0; i < VisualData->Materials.Num(); i++)
    {
        MeshComponent->SetMaterial(i, VisualData->Materials[i]);
    }

    // Visibility
    MeshComponent->SetVisibility(true);

    PlacedItem = SelectedItem;
    bHasItem = true;

    MainCharacter->RemoveSelectedItemFromInventory();
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

bool APlaceSpot::GetHasItem() const
{
    return bHasItem;
}

FItemData APlaceSpot::GetPlacedItem() const
{
    return PlacedItem;
}

void APlaceSpot::ShowPreview()
{
    AMainCharacter *MainCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (MainCharacter == nullptr)
        return;

    FItemData SelectedItem = MainCharacter->GetSelectedItem();
    if (IsAcceptable(SelectedItem.ItemID) == false)
        return;

    FItemVisualData *VisualData =
        ItemVisualDataTable->FindRow<FItemVisualData>(SelectedItem.ItemID, TEXT("APlaceSpot::TogglePreview()"));
    if (VisualData == nullptr)
        return;

    MeshComponent->SetStaticMesh(VisualData->Mesh);

    for (int32 i = 0; i < VisualData->Materials.Num(); i++)
    {
        MeshComponent->SetMaterial(i, PreviewMaterial);
    }

    MeshComponent->SetVisibility(true);
}

void APlaceSpot::HidePreview()
{
    if (bHasItem == false)
        MeshComponent->SetVisibility(false);
}