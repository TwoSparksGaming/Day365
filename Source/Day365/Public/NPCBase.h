#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Time/TimeAwareInterface.h"
#include "NPCBase.generated.h"

USTRUCT(BlueprintType)
struct FNPCTimeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FName TimeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FVector Location = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    UAnimSequence *Pose = nullptr;
};

UCLASS()
class DAY365_API ANPCBase : public ACharacter, public ITimeAwareInterface
{
    GENERATED_BODY()

  public:
    ANPCBase();

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    TArray<FNPCTimeData> TimeDataList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    class ATimeManager *TimeManager = nullptr;

  public:
    virtual void BeginPlay() override;
    virtual void OnTimeChanged_Implementation(const FName &NewState) override;

  private:
    void ApplyNPCData(const FNPCTimeData &Data);
};