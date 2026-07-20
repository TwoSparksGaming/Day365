#include "PaintPuzzleManager.h"
#include "Interaction/PlaceSpot.h"
#include "Time/TextureSwap.h"

APaintPuzzleManager::APaintPuzzleManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void APaintPuzzleManager::CheckPuzzle() const
{
    TArray<FName> CurrentCombination = GetCurrentCombination();
    const FCombinationResult *Result = FindCombinationResult(CurrentCombination);

    if (TargetActor == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("TargetActor is not set."));
        return;
    }

    UStaticMeshComponent *MeshComp = TargetActor->GetStaticMeshComponent();
    if (MeshComp == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("MeshComp is nullptr."));
        return;
    }

    if (Result == nullptr)
    {
        if (WrongPaintResult == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("WrongPaintResult is not set."));
            return;
        }

        MeshComp->SetMaterial(0, WrongPaintResult);
    }
    else
    {
        MeshComp->SetMaterial(0, Result->ResultMaterial);
    }
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
    CombinationTable->GetAllRows<FCombinationResult>(TEXT("APaintPuzzleManager::FindCombinationResult"), AllRows);

    if (AllRows.IsEmpty() == true)
    {
        UE_LOG(LogTemp, Warning, TEXT("No row found in CombinationTable."));
        return nullptr;
    }

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
