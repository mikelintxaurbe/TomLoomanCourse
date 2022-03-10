// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseProjectile.h"
#include "STeleportProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASTeleportProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, Category = "TeleportProjectile", Meta = (ClampMin = "0"))
    float InstigatorTeleportDelay = 0.0f;
	
protected:

    virtual void Explode() override;

    virtual void DestroyProjectile() override;

private:

    void OnInstigatorTeleportDelayTimerElapsed();

    FTimerHandle TimerHandle_InstigatorTeleportDelay;

};
