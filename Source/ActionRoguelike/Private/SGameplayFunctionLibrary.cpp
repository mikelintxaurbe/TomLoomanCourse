// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "ActionRoguelike/Public/SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
    USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
    if (AttributeComp != nullptr)
    {
        if (DamageAmount > 0.0f)
        {
            DamageAmount *= -1.0f;
        }

        return AttributeComp->ApplyHealthChange(DamageCauser, DamageAmount);
    }

    return false;
}

bool USGameplayFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
    if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
    {
        UPrimitiveComponent* HitComp = HitResult.GetComponent();
        if (HitComp != nullptr && HitComp->IsSimulatingPhysics(HitResult.BoneName))
        {
            constexpr float ImpulseStrength = 300000.0f;
            HitComp->AddImpulseAtLocation(-1.0f * HitResult.ImpactNormal * ImpulseStrength, HitResult.ImpactPoint, HitResult.BoneName);
        }

        return true;
    }

    return false;
}
