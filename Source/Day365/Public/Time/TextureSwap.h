#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Time/TimeAwareInterface.h"
#include "GameTypes.h"
#include "TextureSwap.generated.h"

UCLASS()
class DAY365_API ATextureSwap : public AActor, public ITimeAwareInterface
{
    GENERATED_BODY()

  public:
    ATextureSwap();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeSwap")
    UMaterialInterface *PastMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeSwap")
    UMaterialInterface *PresentMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeSwap")
    UMaterialInterface *FutureMaterial = nullptr;

  public:
    virtual void OnTimeChanged_Implementation(ETimeState NewState) override;

    void SetMaterials(UMaterialInterface *Material);
    void SetMaterials(UMaterialInterface *Past, UMaterialInterface *Present, UMaterialInterface *Future);
};