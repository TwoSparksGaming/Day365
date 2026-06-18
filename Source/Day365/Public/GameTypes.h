#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "GameTypes.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
    Acquire UMETA(DisplayName = "Acquire"),
    Puzzle UMETA(DisplayName = "Puzzle"),
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

UENUM(BlueprintType)
enum class ETimeState : uint8
{
    Present UMETA(DisplayName = "Present"),
    Past UMETA(DisplayName = "Past"),
    Future UMETA(DisplayName = "Future")
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
    FText ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D *ItemIcon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EInteractionType InteractionType = EInteractionType::NoInteraction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType ItemType = EItemType::Tool;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bIsUsable = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UStaticMesh *ItemMesh = nullptr;
};