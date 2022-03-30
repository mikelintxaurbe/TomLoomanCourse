// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "ActionRoguelike/Public/SGameModeBase.h"

namespace
{
    constexpr float HealthMin = 0.0f;
}

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier for Attribute Component."), ECVF_Cheat);

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

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
    return ApplyHealthChange(InstigatorActor, -1.0f * GetHealthMax());
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

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
    if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
    {
        return false;
    }

    if (Delta < 0.0f)
    {
        const float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

        Delta *= DamageMultiplier;
    }

    float PrevHealth = Health;

    Health = FMath::Clamp(Health + Delta, HealthMin, HealthMax);
    Delta = Health - PrevHealth;

    const bool Changed = Health != PrevHealth;

    if (Changed)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] USAttributeComponent::ApplyHealthChange(%f)"), *GetNameSafe(this), Delta);
        UE_LOG(LogTemp, Log, TEXT("[%s]     - Health = %f -> %f"), *GetNameSafe(this), Health, Health + Delta);

        OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

        // Died
        if (Delta < 0.0f && Health == HealthMin)
        {
            ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>(); // auth == only on the server
            if (GM != nullptr)
            {
                GM->OnActorKilled(GetOwner(), InstigatorActor);
            }
        }
    }

    return Changed;
}

void USAttributeComponent::SetHealth(float Value)
{
    // No instigator because this comes directly from the editor
    ApplyHealthChange(nullptr, Value - Health);
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
    if (FromActor != nullptr)
    {
        return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
    }

    return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
    USAttributeComponent* AttributeComp = GetAttributes(Actor);
    if (AttributeComp != nullptr)
    {
        return AttributeComp->IsAlive();
    }

    return false;
}

