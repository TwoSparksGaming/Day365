// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InteractableBase.h"
#include "TimeManager.h"
#include "PaintPuzzleManager.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
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

// ──────────────────────────────────────────
// Inventory
// ──────────────────────────────────────────
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

// ──────────────────────────────────────────
// Interaction
// ──────────────────────────────────────────
void AMainCharacter::PerformInteractionTrace()
{
    if (bIsInteracting == true)
    {
        SetInteractWidgetVisible(false);
        if (CurrentPlaceSpot != nullptr)
            CurrentPlaceSpot->HidePreview();
        return;
    }

    // 카메라 위치, 방향 가져오기
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

    // PlaceSpot 먼저 체크
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

            PlaceSpot->ShowPreview();
            CurrentPlaceSpot = PlaceSpot;
        }

        return;
    }

    // InteractableBase 체크
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

    // 둘 다 아니면 초기화
    CurrentTarget = nullptr;
    SetInteractWidgetVisible(false);
    if (CurrentPlaceSpot != nullptr)
        CurrentPlaceSpot->HidePreview();
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

// ──────────────────────────────────────────
// Time
// ──────────────────────────────────────────
bool AMainCharacter::PreChangeTime(const bool bToFuture)
{
    if (bIsTimeChanging == true)
        return false;

    bIsTimeChanging = true;

    if (bToFuture == true)
    {
        if (CurrentTimeState == ETimeState::Past)
        {
            TargetTimeState = ETimeState::Present;
        }
        else if (CurrentTimeState == ETimeState::Present)
        {
            TargetTimeState = ETimeState::Future;
        }
        else if (CurrentTimeState == ETimeState::Future)
        {
            bIsTimeChanging = false;
            return false;
        }
    }
    else
    {
        if (CurrentTimeState == ETimeState::Past)
        {
            bIsTimeChanging = false;
            return false;
        }
        else if (CurrentTimeState == ETimeState::Present)
        {
            TargetTimeState = ETimeState::Past;
        }
        else if (CurrentTimeState == ETimeState::Future)
        {
            TargetTimeState = ETimeState::Present;
        }
    }

    return true;
}

void AMainCharacter::ChangeTime()
{
    if (CurrentTimeState == TargetTimeState)
        return;

    CurrentTimeState = TargetTimeState;

    TArray<AActor *> TimeManagers;
    TArray<AActor *> PaintPuzzleManagers;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATimeManager::StaticClass(), TimeManagers);
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APaintPuzzleManager::StaticClass(), PaintPuzzleManagers);

    if (TimeManagers.IsEmpty() == true)
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeManager not found"));
        return;
    }

    if (PaintPuzzleManagers.IsEmpty() == true)
    {
        UE_LOG(LogTemp, Warning, TEXT("PaintPuzzleManager not found"));
        return;
    }

    ATimeManager *TimeManager = Cast<ATimeManager>(TimeManagers[0]);
    if (TimeManager == nullptr)
        return;

    APaintPuzzleManager *PaintPuzzleManager = Cast<APaintPuzzleManager>(PaintPuzzleManagers[0]);
    if (PaintPuzzleManager == nullptr)
        return;

    TimeManager->NotifyTimeChanged(TargetTimeState);
    PaintPuzzleManager->CheckCombination();

    bIsTimeChanging = false;
}
