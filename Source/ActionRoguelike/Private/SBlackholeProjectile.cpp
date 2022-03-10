// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

void ASBlackholeProjectile::OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Don't call Super implementation, we want to do all checks from BP
}
