// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/InteractableBase.h"
#include "Time/TimeManager.h"
#include "PaintPuzzleManager.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

// Initialize
void AMainCharacter::Initialize()
{
    // TimeManager
    if (TimeManager == nullptr)
    {
        TArray<AActor *> TimeManagers;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeManager::StaticClass(), TimeManagers);

        if (TimeManagers.Num() == 1)
        {
            TimeManager = Cast<ATimeManager>(TimeManagers[0]);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("TimeManager not found or more than one in level."));
        }
    }
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();

    Initialize();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    PerformInteractionTrace();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent *EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInput == nullptr)
        return;

    EnhancedInput->BindAction(IA_Interact, ETriggerEvent::Started, this, &AMainCharacter::OnInteract);
    EnhancedInput->BindAction(IA_Cancel, ETriggerEvent::Started, this, &AMainCharacter::CancelInteraction);
}

// ---------------------------------------------------------------------------------------------
// Inventory
// ---------------------------------------------------------------------------------------------
void AMainCharacter::AddItemToInventory(FItemData NewItem)
{
    InventoryItems.Add(NewItem);
    OnInventoryUpdated();
}

void AMainCharacter::RemoveItemFromInventory(FItemData Item)
{
    InventoryItems.RemoveAll([&Item](const FItemData &Element) { return Element.ItemID == Item.ItemID; });
    OnInventoryUpdated();
}

void AMainCharacter::RemoveSelectedItemFromInventory()
{
    RemoveItemFromInventory(GetSelectedItem());
}

void AMainCharacter::AquirePocketWatch()
{
    if (bHaveClock == true)
    {
        UE_LOG(LogTemp, Error, TEXT("Already have pocket watch and try to get it."));
        return;
    }

    bHaveClock = true;
    OnClockAcquired();
}

FItemData AMainCharacter::GetSelectedItem() const
{
    if (InventoryItems.IsValidIndex(SelectedIndex) == true)
        return InventoryItems[SelectedIndex];

    return FItemData();
}

// ---------------------------------------------------------------------------------------------
// Interaction
// ---------------------------------------------------------------------------------------------
void AMainCharacter::PerformInteractionTrace()
{
    if (bIsInteracting == true)
    {
        SetInteractWidgetVisible(false);
        if (CurrentPlaceSpot != nullptr)
            CurrentPlaceSpot->HidePreview();
        return;
    }

    // LineTrace
    APlayerCameraManager *CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
    if (CameraManager == nullptr)
        return;

    FVector Start = CameraManager->GetCameraLocation();
    FVector End = Start + CameraManager->GetActorForwardVector() * TraceLength;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.bTraceComplex = true;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);
    if (bHit == false)
    {
        SetInteractWidgetVisible(false);
        CurrentTarget = nullptr;
        if (CurrentPlaceSpot != nullptr)
            CurrentPlaceSpot->HidePreview();
    }

    AActor *HitActor = HitResult.GetActor();
    if (HitActor == nullptr)
        return;

    // PlaceSpot
    APlaceSpot *PlaceSpot = Cast<APlaceSpot>(HitActor);
    if (PlaceSpot != nullptr)
    {
        if (PlaceSpot->GetHasItem() == true)
        {
            if (PlaceSpot->Execute_CanInteract(PlaceSpot) == true)
            {
                CurrentTarget = HitActor;
                SetInteractWidgetVisible(true);
            }
            else
            {
                CurrentTarget = nullptr;
                SetInteractWidgetVisible(false);
            }
        }
        else
        {
            FItemData SelectedItem = GetSelectedItem();
            if (SelectedItem.IsValid() == false)
                return;

            if (CurrentPlaceSpot != PlaceSpot)
            {
                if (CurrentPlaceSpot != nullptr)
                    CurrentPlaceSpot->HidePreview();

                CurrentPlaceSpot = PlaceSpot;
                CurrentPlaceSpot->ShowPreview();
            }
        }

        return;
    }

    // InteractableBase
    AInteractableBase *Interactable = Cast<AInteractableBase>(HitActor);
    if (Interactable != nullptr)
    {
        if (Interactable->Execute_CanInteract(Interactable) == true)
        {
            CurrentTarget = HitActor;
            SetInteractWidgetVisible(true);
        }
        else
        {
            CurrentTarget = nullptr;
            SetInteractWidgetVisible(false);
        }
        return;
    }

    // Interaction Hint
    CurrentTarget = nullptr;
    SetInteractWidgetVisible(false);
    if (CurrentPlaceSpot != nullptr)
    {
        CurrentPlaceSpot->HidePreview();
        CurrentPlaceSpot = nullptr;
    }
}

void AMainCharacter::OnInteract()
{
    if (bIsInteracting == true)
        return;

    if (CurrentTarget != nullptr)
    {
        IInteractableInterface::Execute_Interact(CurrentTarget);
        return;
    }

    if (CurrentPlaceSpot != nullptr)
    {
        IInteractableInterface::Execute_Interact(CurrentPlaceSpot);
        return;
    }
}

void AMainCharacter::SetCurrentInteractObject(AActor *object)
{
    CurrentInteractObject = object;
    bIsInteracting = true;
}

void AMainCharacter::CancelInteraction()
{
    if (bIsInteracting == false)
        return;

    if (CurrentInteractObject == nullptr)
        return;

    SwitchCamera(nullptr, 1.f, false);
    CurrentInteractObject = nullptr;
    bIsInteracting = false;
}

// ---------------------------------------------------------------------------------------------
// Time
// ---------------------------------------------------------------------------------------------
FName AMainCharacter::GetCurrentTimeState() const
{
    if (TimeManager == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeManager is not initialized."));
        return NAME_None;
    }

    return TimeManager->GetCurrentTimeState();
}

bool AMainCharacter::PreChangeTime(const bool bToFuture)
{
    if (bIsTimeChanging == true)
        return false;

    if (TimeManager == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeManager is not set"));
        return false;
    }

    if (bToFuture == true && TimeManager->CanMoveForward() == false)
        return false;

    if (bToFuture == false && TimeManager->CanMoveBackward() == false)
        return false;

    bIsTimeChanging = true;
    return true;
}

void AMainCharacter::ChangeTime()
{
    if (TimeManager == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeManager is not set."));
        return;
    }

    if (TimeManager->GetMovingTimeState() == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeManager::MovingTimeState is NAME_None."));
        return;
    }

    TimeManager->ChangeTime();
    bIsTimeChanging = false;
}
