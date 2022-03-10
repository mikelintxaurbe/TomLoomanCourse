// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include <GameFramework/ProjectileMovementComponent.h>
#include <Particles/ParticleSystemComponent.h>

void ASTeleportProjectile::Explode()
{
    Super::Explode();

    GetWorldTimerManager().SetTimer(TimerHandle_InstigatorTeleportDelay, this, &ASTeleportProjectile::OnInstigatorTeleportDelayTimerElapsed, InstigatorTeleportDelay);

    // Stop projectile so that we teleport the instigator to the position where we exploded (otherwise we will continue moving and he will be teleported further away)
    MovementComp->StopMovementImmediately();

    EffectComp->SetActive(false);
    EffectComp->SetVisibility(false);
}

void ASTeleportProjectile::DestroyProjectile()
{
    // Don't call Super::DestroyProjectile() yet, wait for the delay before teleporting our Instigator first
}

void ASTeleportProjectile::OnInstigatorTeleportDelayTimerElapsed()
{
    if (APawn* InstigatorPawn = GetInstigator())
    {
        InstigatorPawn->SetActorLocation(GetActorLocation());
    }

    Super::DestroyProjectile();
}
