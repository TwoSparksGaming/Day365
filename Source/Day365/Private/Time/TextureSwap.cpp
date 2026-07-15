#include "Time/TextureSwap.h"
#include "Components/StaticMeshComponent.h"

ATextureSwap::ATextureSwap()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
}

void ATextureSwap::OnTimeChanged_Implementation(ETimeState NewState)
{
    UMaterialInterface *TargetMaterial = nullptr;

    if (NewState == ETimeState::Past)
    {
        TargetMaterial = PastMaterial;
    }
    else if (NewState == ETimeState::Present)
    {
        TargetMaterial = PresentMaterial;
    }
    else if (NewState == ETimeState::Future)
    {
        TargetMaterial = FutureMaterial;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("TimeState Not Implemented."));
        return;
    }

    if (TargetMaterial == nullptr)
        return;

    for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
    {
        MeshComponent->SetMaterial(i, TargetMaterial);
    }
}

void ATextureSwap::SetMaterials(UMaterialInterface *Material)
{
    if (Material == nullptr)
        return;

    for (int32 i = 0; i < MeshComponent->GetNumMaterials(); i++)
    {
        MeshComponent->SetMaterial(i, Material);
    }
}

void ATextureSwap::SetMaterials(UMaterialInterface *Past, UMaterialInterface *Present, UMaterialInterface *Future)
{
    PastMaterial = Past;
    PresentMaterial = Present;
    FutureMaterial = Future;
}