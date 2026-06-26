#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeAwareInterface.h"
#include "GameTypes.h"
#include "MeshSwap.generated.h"

UCLASS()
class DAY365_API AMeshSwap : public AActor, public ITimeAwareInterface
{
    GENERATED_BODY()

  public:
    AMeshSwap();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *PastMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *PresentMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *FutureMesh;

    UPROPERTY(BlueprintReadWrite, Category = "TimeSwap")
    ETimeState CurrentState = ETimeState::Present;

  public:
    virtual void BeginPlay() override;
    virtual void OnTimeChanged_Implementation(ETimeState NewState) override;
};