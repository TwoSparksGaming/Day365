#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableInterface.h"
#include "Components/StaticMeshComponent.h" // ← 추가
#include "GameTypes.h"
#include "InteractableBase.generated.h"

UCLASS()
class DAY365_API AInteractableBase : public AActor, public IInteractableInterface
{
    GENERATED_BODY()

  public:
    AInteractableBase();

  protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bCanInteract = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    bool bIsDeactivated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FItemData ItemData;

  public:
    // IInteractableInterface 구현
    virtual void Interact_Implementation() override;
    virtual bool CanInteract_Implementation() override;

  protected:
    // 블루프린트에서 Override 가능한 함수들
    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void OnAcquire(class ACharacter *PlayerCharacter);

    UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
    void OnPuzzleInteract(class ACharacter *PlayerCharacter);
};