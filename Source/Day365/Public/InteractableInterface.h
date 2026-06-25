#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class DAY365_API IInteractableInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    bool CanInteract();
};
