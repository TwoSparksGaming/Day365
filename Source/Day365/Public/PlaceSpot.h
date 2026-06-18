#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
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

  public:
    // IInteractableInterface 구현
    virtual void Interact_Implementation() override;
    virtual bool CanInteract_Implementation() override;

    // 아이템 배치 가능 여부 확인
    UFUNCTION(BlueprintCallable, Category = "PlaceSpot")
    bool IsAcceptable(FName ItemID) const;

    // 아이템 놓기
    UFUNCTION(BlueprintCallable, Category = "PlaceSpot")
    void PlaceItem();

    // 아이템 집기
    UFUNCTION(BlueprintCallable, Category = "PlaceSpot")
    void PickUpItem();

    // 게터
    UFUNCTION(BlueprintPure, Category = "PlaceSpot")
    bool GetHasItem() const
    {
        return bHasItem;
    }

    UFUNCTION(BlueprintPure, Category = "PlaceSpot")
    FItemData GetPlacedItem() const
    {
        return PlacedItem;
    }

  protected:
    // 블루프린트에서 구현
    UFUNCTION(BlueprintImplementableEvent, Category = "PlaceSpot")
    void OnToggleItem(FItemData NewItem, bool isShow = true);

    UFUNCTION(BlueprintImplementableEvent, Category = "PlaceSpot")
    void OnTogglePreview(bool isShow);

    UFUNCTION(BlueprintImplementableEvent, Category = "PlaceSpot")
    void OnCorrectItemPlaced();
};
