// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
    Health = 100.0f;
}


bool USAttributeComponent::IsAlive() const
{
    return Health > 0.0f;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] USAttributeComponent::ApplyHealthChange(%f)"), *GetNameSafe(this), Delta);
    UE_LOG(LogTemp, Log, TEXT("[%s]     - Health = %f -> %f"), *GetNameSafe(this), Health, Health + Delta);

    Health += Delta;

    OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

    return true;
}

