#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleManager.h"
#include "Engine/StaticMeshActor.h"
#include "PaintPuzzleManager.generated.h"

class APlaceSpot;
class ATextureSwap;

USTRUCT(BlueprintType)
struct FCombinationResult : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    TArray<FName> ItemIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    UMaterialInterface *ResultMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    bool bIsCorrect = false;
};

UCLASS()
class DAY365_API APaintPuzzleManager : public APuzzleManager
{
    GENERATED_BODY()

  public:
    APaintPuzzleManager();

  protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    APlaceSpot *PlaceSpot_A = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    APlaceSpot *PlaceSpot_B = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    APlaceSpot *PlaceSpot_C = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    AStaticMeshActor *TargetActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    UMaterialInterface *WrongMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    UMaterialInterface *OriginalMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    UDataTable *CombinationTable = nullptr;

  public:
    virtual void CheckPuzzle() const override;
    virtual void RevertPuzzle() const override;

  private:
    TArray<FName> GetCurrentCombination() const;

    const FCombinationResult *FindCombinationResult(const TArray<FName> &Combination) const;
};