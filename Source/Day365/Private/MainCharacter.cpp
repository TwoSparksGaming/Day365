// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
}

void AMainCharacter::AddItemToInventory(FItemData NewItem)
{
    InventoryItems.Add(NewItem);
    OnInventoryUpdated();
}

void AMainCharacter::RemoveItemFromInventory(FItemData Item)
{
    InventoryItems.RemoveAll([&Item](const FItemData &Element) { return Element.ItemID == Item.ItemID; });
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

void AMainCharacter::PerformInteractionTrace()
{
    // bIsInteracting »óÅÂ¸é ÈùÆ® ¼û±â°í return
    if (bIsInteracting == true)
    {
        SetInteractWidgetVisible(false);
        ClearPreview();
        return;
    }

    // Ä«¸Þ¶ó À§Ä¡, ¹æÇâ °¡Á®¿À±â
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
        // ÀÌÀü Å¸°Ù À±°û¼± ²ô±â
        if (CurrentTarget != nullptr)
        {
            // À±°û¼± ²ô±â ¡æ ºí·çÇÁ¸°Æ®·Î À§ÀÓ
            SetInteractWidgetVisible(false);
            CurrentTarget = nullptr;
        }
        ClearPreview();
        return;
    }

    AActor *HitActor = HitResult.GetActor();
    if (HitActor == nullptr)
        return;

    IInteractableInterface *Interactable = Cast<IInteractableInterface>(HitActor);
    if (Interactable == nullptr)
        return;

    if (Interactable->Execute_CanInteract(HitActor) == false)
        return;

    CurrentTarget = HitActor;
    SetInteractWidgetVisible(true);

    UpdatePlaceSpotPreview(HitActor, GetSelectedItem());
}

FItemData AMainCharacter::GetSelectedItem() const
{
    if (InventoryItems.IsValidIndex(SelectedIndex) == true)
        return InventoryItems[SelectedIndex];

    return FItemData();
}

void AMainCharacter::OnInteract()
{
    if (CurrentTarget == nullptr)
        return;

    if (bIsInteracting == true)
        return;

    IInteractableInterface::Execute_Interact(CurrentTarget);
}