#include "Interaction/InteractableWithCamera.h"
#include "Camera/CameraComponent.h"

AInteractableWithCamera::AInteractableWithCamera()
{
    InteractCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("InteractCamera"));
    InteractCamera->SetupAttachment(RootComponent);
}

UCameraComponent *AInteractableWithCamera::GetInteractCamera() const
{
    return InteractCamera;
}