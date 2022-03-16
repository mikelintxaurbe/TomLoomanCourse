// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include <Components/SphereComponent.h>

#include "ActionRogueLike/Public/SAttributeComponent.h"

bool ASHealthPotion::CanInteractWith(const APawn* InstigatorPawn) const
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASHealthPotion::CanInteractWith()"), *GetNameSafe(this));

    if (CurrentActiveState != EActiveState::Active)
    {
        return false;
    }

    if (USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
    {
        return AttributeComponent->GetHealth() < AttributeComponent->GetHealthMax();
    }

    return true;
}

void ASHealthPotion::DoInteraction(APawn* InstigatorPawn)
{
    Super::DoInteraction(InstigatorPawn);

    UE_LOG(LogTemp, Log, TEXT("[%s] ASHealthPotion::DoInteraction()"), *GetNameSafe(this));

    if (USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
    {
        AttributeComponent->ApplyHealthChange(HealthRestored);
    }

    SetActiveState(EActiveState::Inactive);

    FTimerDelegate TimerCallback;
    TimerCallback.BindLambda([this]()
    {
        SetActiveState(EActiveState::Active);
    });

    FTimerHandle TimerHandle;
    constexpr bool bDoesLoop = false;
    GetWorldTimerManager().SetTimer(TimerHandle, TimerCallback, InactiveDurationSeconds, bDoesLoop);
}

void ASHealthPotion::SetActiveState(const EActiveState NewActiveState)
{
    if (CurrentActiveState != NewActiveState)
    {
        CurrentActiveState = NewActiveState;

        const bool ShowMesh = NewActiveState == EActiveState::Active;
        StaticMeshComp->SetVisibility(ShowMesh, true);
    }
}

