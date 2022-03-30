// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include <Kismet/GameplayStatics.h>

#include "ActionRoguelike/Public/SAttributeComponent.h"
#include "ActionRoguelike/Public/SGameplayFunctionLibrary.h"

void ASMagicProjectile::OnOverlapActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
    Super::OnOverlapActor(OtherActor, OtherComp, Hit);

    if (OtherActor != nullptr)
    {
        /*USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp != nullptr)
        {
            ensure(Damage > 0.0f);
            AttributeComp->ApplyHealthChange(GetInstigator(), -1.0f * Damage);
        }*/

        USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, Hit);
    }
}

void ASMagicProjectile::Explode()
{
    Super::Explode();

    const float CameraShakeIgnoreCamerasRadius = 0.0f;
    UGameplayStatics::PlayWorldCameraShake(this, CameraShake, GetActorLocation(), CameraShakeIgnoreCamerasRadius, CameraShakeRadius);
}

