// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleManager.generated.h"

UCLASS(Abstract)
class DAY365_API APuzzleManager : public AActor
{
    GENERATED_BODY()

  public:
    APuzzleManager();

    virtual void CheckPuzzle() const
    {
        return;
    }
};