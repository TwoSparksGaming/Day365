#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameTypes.h"
#include "InputActionValue.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class DAY365_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()
  private:
  public:
    AMainCharacter();

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

    // Interaction
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractDistance = 200.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    bool bIsInteracting = false;

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

  public:
    // Inventory Func
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void AddItemToInventory(FItemData NewItem);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveItemFromInventory(FItemData Item);

    UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
    void OnInventoryUpdated();

    UFUNCTION(BlueprintCallable, Category = "Time")
    void AquirePocketWatch();

    // ½Ã°è È¹µæ ½Ã UI Ã³¸®
    UFUNCTION(BlueprintImplementableEvent, Category = "Time")
    void OnClockAcquired();

    // Input
    void Move(const FInputActionValue &Value);
    void Look(const FInputActionValue &Value);
    void Interact();

    // Getter
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

    UFUNCTION(BlueprintPure, Category = "Camera")
    UCameraComponent *GetCameraComponent() const
    {
        return CameraComponent;
    }
};