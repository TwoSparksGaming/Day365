#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "PuzzleManager.h"
#include "GameTypes.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    Acquire UMETA(DisplayName = "Acquire"),
    ZoomInPuzzle UMETA(DisplayName = "ZoomInPuzzle"),
    NoInteraction UMETA(DisplayName = "No Interaction")
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Watch UMETA(DisplayName = "Watch"),
    Tool UMETA(DisplayName = "Tool"),
    Note UMETA(DisplayName = "Note"),
    Special UMETA(DisplayName = "Special")
};

USTRUCT(BlueprintType)
struct FItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EInteractionType InteractionType = EInteractionType::NoInteraction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Tool;

    bool IsValid() const
    {
        return ItemID != NAME_None;
    }
};

USTRUCT(BlueprintType)
struct FItemVisualData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UStaticMesh *Mesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TArray<UMaterialInterface *> Materials;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    UTexture2D *Icon = nullptr;
};

USTRUCT(BlueprintType)
struct FTimeBranch
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName TargetTimeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    APuzzleManager *PuzzleManager = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName CorrectTimeState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName WrongTimeState;
};

USTRUCT(BlueprintType)
struct FTimeTransition : public FTableRowBase
{
    GENERATED_BODY()

    // 나중에 시간 바뀔때 레벨 자체가 바뀐다거나, 조건에 따라 분기할때 필요할듯 해서 남겨둠.
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    // FName FromState;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    // FTimeBranch Forward;

    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    // FTimeBranch Backward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName Backward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName Current;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
    FName Forward;
};