#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Time/TimeAwareInterface.h"
#include "TimeObject.generated.h"

USTRUCT(BlueprintType)
struct FTimeStateData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName TimeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    UStaticMesh *Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    TArray<UMaterialInterface *> Materials;
};

UCLASS()
class DAY365_API ATimeObject : public AActor, public ITimeAwareInterface
{
    GENERATED_BODY()

  public:
    ATimeObject();

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent *MeshComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    TArray<FTimeStateData> TimeStateDataList;

  public:
    virtual void BeginPlay() override;
    virtual void OnTimeChanged_Implementation(const FName &NewTimeState) override;

  private:
    void ApplyTimeStateData(const FTimeStateData &Data);
};