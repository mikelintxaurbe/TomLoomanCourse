// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include <Components/SphereComponent.h>

#include "ActionRogueLike/Public/SAttributeComponent.h"

bool ASHealthPotion::CanInteractWith(const APawn* InstigatorPawn) const
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASHealthPotion::CanInteractWith()"), *GetNameSafe(this));

    if (USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
    {
        return AttributeComponent->GetHealth() < AttributeComponent->GetHealthMax();
    }

    return false;
}

void ASHealthPotion::DoInteraction(APawn* InstigatorPawn)
{
    Super::DoInteraction(InstigatorPawn);

    UE_LOG(LogTemp, Log, TEXT("[%s] ASHealthPotion::DoInteraction()"), *GetNameSafe(this));

    USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
    if (ensure(AttributeComponent != nullptr))
    {
        AttributeComponent->ApplyHealthChange(HealthRestored);
    }
}

