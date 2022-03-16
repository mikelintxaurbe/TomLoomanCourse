// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

namespace
{
    constexpr float HealthMin = 0.0f;
}

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
    HealthMax = 100.0f;
    Health = HealthMax;
}

void USAttributeComponent::BeginPlay()
{
    Super::BeginPlay();

    // Clamp HealthMax to a valid value and update Health again just in case
    HealthMax = FMath::Clamp(HealthMax, HealthMin, TNumericLimits<float>::Max());
    
    if (HealthMax < Health)
    {
        Health = HealthMax;
    }
}

bool USAttributeComponent::IsAlive() const
{
    return Health > 0.0f;
}

float USAttributeComponent::GetHealth() const
{
    return Health;
}

float USAttributeComponent::GetHealthMax() const
{
    return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
    float PrevHealth = Health;

    Health = FMath::Clamp(Health + Delta, HealthMin, HealthMax);
    Delta = Health - PrevHealth;

    const bool Changed = Health != PrevHealth;

    if (Changed)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] USAttributeComponent::ApplyHealthChange(%f)"), *GetNameSafe(this), Delta);
        UE_LOG(LogTemp, Log, TEXT("[%s]     - Health = %f -> %f"), *GetNameSafe(this), Health, Health + Delta);

        OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
    }

    return Changed;
}

void USAttributeComponent::SetHealth(float Value)
{
    ApplyHealthChange(Value - Health);
}

