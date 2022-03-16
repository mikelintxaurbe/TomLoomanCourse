// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseInteractive.h"

#include <Components/SphereComponent.h>

ASBaseInteractive::ASBaseInteractive()
{
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComp");
    RootComponent = StaticMeshComp;

    InteractiveSphereCollision = CreateDefaultSubobject<USphereComponent>("InteractiveSphereCollision");
    InteractiveSphereCollision->SetCollisionProfileName("Interactive");
    InteractiveSphereCollision->SetAutoActivate(true);
    InteractiveSphereCollision->SetGenerateOverlapEvents(true);
    InteractiveSphereCollision->SetupAttachment(RootComponent);
}

bool ASBaseInteractive::CanInteractWith(const APawn* InstigatorPawn) const
{
    return true;
}

void ASBaseInteractive::DoInteraction(APawn* InstigatorPawn)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseInteractive::DoInteraction()"), *GetNameSafe(this));
}

void ASBaseInteractive::Interact_Implementation(APawn* InstigatorPawn)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseInteractive::Interact_Implementation()"), *GetNameSafe(this));

    if (CanInteractWith(InstigatorPawn))
    {
        DoInteraction(InstigatorPawn);
    }
}
