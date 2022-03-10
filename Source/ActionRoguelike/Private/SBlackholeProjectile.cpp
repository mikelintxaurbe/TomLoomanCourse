// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

bool ASBlackholeProjectile::IsOverlapValid(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // Ignore all overlaps from code, we're doing the overlap checks in Blueprint for now
    return false;
}

