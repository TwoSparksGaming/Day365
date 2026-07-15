#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractableInterface.h"
#include "GameTypes.h"
#include "PlaceSpot.generated.h"

UCLASS()
class DAY365_API APlaceSpot : public AActor, public IInteractableInterface
{
    GENERATED_BODY()

  public:
    APlaceSpot();

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceSpot")
    TArray<FName> AcceptableItems;

    UPROPERTY(BlueprintReadWrite, Category = "PlaceSpot")
    FItemData PlacedItem;

    UPROPERTY(BlueprintReadWrite, Category = "PlaceSpot")
    bool bHasItem = false;

    UPROPERTY(EditAnywhere, Category = "PlaceSpot")
    UDataTable *ItemVisualDataTable = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlaceSpot")
    UMaterialInterface *PreviewMaterial = nullptr;

  public:
    // IInteractableInterface ����
    virtual void Interact_Implementation() override;
    virtual bool CanInteract_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "PlaceSpot")
    bool IsAcceptable(FName ItemID) const;

    UFUNCTION(BlueprintCallable, Category = "PlaceSpot")
    void PlaceItem();

    UFUNCTION(BlueprintCallable, Category = "PlaceSpot")
    void PickUpItem();

    UFUNCTION(BlueprintPure, Category = "PlaceSpot")
    bool GetHasItem() const;

    UFUNCTION(BlueprintPure, Category = "PlaceSpot")
    FItemData GetPlacedItem() const;

    void ShowPreview();
    void HidePreview();

  protected:
    // ��������Ʈ���� ����
    UFUNCTION(BlueprintImplementableEvent, Category = "PlaceSpot")
    void OnToggleItem(FItemData NewItem, bool isShow = true);

    UFUNCTION(BlueprintImplementableEvent, Category = "PlaceSpot")
    void OnTogglePreview(bool isShow);

    UFUNCTION(BlueprintImplementableEvent, Category = "PlaceSpot")
    void OnCorrectItemPlaced();
};
