#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"

#include "GameTypes.h"
#include "InteractableInterface.h"
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
    // 자식 클래스에서 Override해서 카메라 반환
    virtual UCameraComponent *GetInteractCamera() const
    {
        return nullptr;
    }
};