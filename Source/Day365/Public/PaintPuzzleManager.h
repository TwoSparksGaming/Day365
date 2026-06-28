#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameTypes.h"
#include "PaintPuzzleManager.generated.h"

class APlaceSpot;
class ATextureSwap;

UCLASS()
class DAY365_API APaintPuzzleManager : public AActor
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
    ATextureSwap *TargetTextureSwap = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
    UDataTable *CombinationTable = nullptr;

  public:
    UFUNCTION(BlueprintCallable, Category = "Puzzle")
    void CheckCombination();

  private:
    TArray<FName> GetCurrentCombination() const;

    const FCombinationResult *FindCombinationResult(const TArray<FName> &Combination) const;
};