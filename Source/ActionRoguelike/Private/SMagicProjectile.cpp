// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "ActionRoguelike/Public/SAttributeComponent.h"

void ASMagicProjectile::OnOverlapActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
    Super::OnOverlapActor(OtherActor, OtherComp, Hit);

    if (OtherActor != nullptr)
    {
        USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp != nullptr)
        {
            AttributeComp->ApplyHealthChange(Damage);
        }
    }
}

