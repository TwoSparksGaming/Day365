#pragma once

#include "CoreMinimal.h"
#include "InteractableBase.h"
#include "InteractableWithCamera.generated.h"

class UCameraComponent;

UCLASS()
class DAY365_API AInteractableWithCamera : public AInteractableBase
{
    GENERATED_BODY()

  public:
    AInteractableWithCamera();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    UCameraComponent *InteractCamera;

    virtual UCameraComponent *GetInteractCamera() const override;
};