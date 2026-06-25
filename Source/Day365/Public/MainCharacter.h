#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameTypes.h"
#include "InputActionValue.h"
#include "Camera/PlayerCameraManager.h"
#include "InteractableInterface.h"
#include "PlaceSpot.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class DAY365_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

  protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override;

    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent *CameraComponent;

    // Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext *IMC_Default;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *IA_Move;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *IA_Look;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *IA_Jump;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *IA_Interact;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction *IA_Cancel;

    // Interaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractDistance = 200.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    bool bIsInteracting = false;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    AActor *CurrentInteractObject = nullptr;

    // Inventory
    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    TArray<FItemData> InventoryItems;

    UPROPERTY(BlueprintReadWrite, Category = "Inventory")
    int32 SelectedIndex = 0;

    // Time State
    UPROPERTY(BlueprintReadWrite, Category = "Time")
    ETimeState CurrentTimeState = ETimeState::Present;

    UPROPERTY(BlueprintReadWrite, Category = "Time")
    bool bHaveClock = false;

    // Blueprint Func
    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void SetInteractWidgetVisible(bool bVisible);

  private:
    // Line Trace
    void PerformInteractionTrace();

    UPROPERTY()
    AActor *CurrentTarget = nullptr;

    UPROPERTY()
    APlaceSpot *CurrentPlaceSpot = nullptr;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float TraceLength = 200.0f;

  public:
    AMainCharacter();

    // Input
    void Move(const FInputActionValue &Value);
    void Look(const FInputActionValue &Value);
    void OnInteract();

    // Getter
    UFUNCTION(BlueprintPure, Category = "Camera")
    UCameraComponent *GetCameraComponent() const
    {
        return CameraComponent;
    }

    // Inventory Func
    UFUNCTION(BlueprintPure, Category = "Inventory")
    TArray<FItemData> GetInventoryItems() const
    {
        return InventoryItems;
    }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetSelectedIndex() const
    {
        return SelectedIndex;
    }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    FItemData GetSelectedItem() const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItemToInventory(FItemData NewItem);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveItemFromInventory(FItemData Item);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveSelectedItemFromInventory();

    // Interaction
    UFUNCTION(BlueprintCallable, Category = "Time")
    void AquirePocketWatch();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void SetCurrentInteractObject(AActor *object);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void CancelInteraction();

    // Blueprint Func
    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
    void OnInventoryUpdated();

    UFUNCTION(BlueprintImplementableEvent, Category = "Time")
    void OnClockAcquired();

    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
    void SwitchCamera(UCameraComponent *TargetCamera, float BlendTime, bool bIsZoomIn);
};