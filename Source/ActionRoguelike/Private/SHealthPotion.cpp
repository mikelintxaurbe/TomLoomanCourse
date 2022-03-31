// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include <Components/SphereComponent.h>

#include "ActionRogueLike/Public/SAttributeComponent.h"
#include "ActionRoguelike/Public/SPlayerStateBase.h"

bool ASHealthPotion::CanInteractWith(const APawn* InstigatorPawn) const
{
    //UE_LOG(LogTemp, Log, TEXT("[%s] ASHealthPotion::CanInteractWith()"), *GetNameSafe(this));

    USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
    if (AttributeComponent == nullptr || AttributeComponent->GetHealth() >= AttributeComponent->GetHealthMax())
    {
        return false;
    }

    ASPlayerStateBase* PlayerStateBase = InstigatorPawn->GetPlayerState<ASPlayerStateBase>();
    if (PlayerStateBase != nullptr && PlayerStateBase->CanAffordCredits(CreditsCost) == false)
    {
        return false;
    }

    return true;
}

void ASHealthPotion::DoInteraction(APawn* InstigatorPawn)
{
    Super::DoInteraction(InstigatorPawn);

    //UE_LOG(LogTemp, Log, TEXT("[%s] ASHealthPotion::DoInteraction()"), *GetNameSafe(this));

    USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
    if (ensure(AttributeComponent != nullptr))
    {
        AttributeComponent->ApplyHealthChange(this, HealthRestored);
    }

    ASPlayerStateBase* PlayerStateBase = InstigatorPawn->GetPlayerState<ASPlayerStateBase>();
    if (PlayerStateBase != nullptr)
    {
        PlayerStateBase->ApplyCreditsChange(-1.0f * CreditsCost);
    }
}

