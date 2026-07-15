#include "PaintPuzzleManager.h"
#include "Interaction/PlaceSpot.h"
#include "Time/TextureSwap.h"

APaintPuzzleManager::APaintPuzzleManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APaintPuzzleManager::CheckCombination()
{
    TArray<FName> CurrentCombination = GetCurrentCombination();
    const FCombinationResult *Result = FindCombinationResult(CurrentCombination);

    if (Result == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No matching combination found"));
        return;
    }

    if (TargetTextureSwap == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetTextureSwap is not set"));
        return;
    }

    TargetTextureSwap->SetMaterials(Result->ResultMaterial);
}

TArray<FName> APaintPuzzleManager::GetCurrentCombination() const
{
    TArray<FName> Combination;

    if (PlaceSpot_A != nullptr && PlaceSpot_A->GetHasItem() == true)
        Combination.Add(PlaceSpot_A->GetPlacedItem().ItemID);

    if (PlaceSpot_B != nullptr && PlaceSpot_B->GetHasItem() == true)
        Combination.Add(PlaceSpot_B->GetPlacedItem().ItemID);

    if (PlaceSpot_C != nullptr && PlaceSpot_C->GetHasItem() == true)
        Combination.Add(PlaceSpot_C->GetPlacedItem().ItemID);

    Combination.Sort([](const FName &A, const FName &B) { return A.ToString() < B.ToString(); });

    return Combination;
}

const FCombinationResult *APaintPuzzleManager::FindCombinationResult(const TArray<FName> &Combination) const
{
    if (CombinationTable == nullptr)
        return nullptr;

    TArray<FCombinationResult *> AllRows;
    CombinationTable->GetAllRows<FCombinationResult>(TEXT("FindCombinationResult"), AllRows);

    for (FCombinationResult *Result : AllRows)
    {
        if (Result == nullptr)
            continue;

        TArray<FName> SortedIDs = Result->ItemIDs;
        SortedIDs.Sort([](const FName &A, const FName &B) { return A.ToString() < B.ToString(); });

        if (SortedIDs == Combination)
        {
            return Result;
        }
    }

    return nullptr;
}
