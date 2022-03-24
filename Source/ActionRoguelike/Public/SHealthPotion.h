// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePowerUp.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASBasePowerUp
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float HealthRestored = 50.0f;

protected:

    bool CanInteractWith(const APawn* InstigatorPawn) const override;

    void DoInteraction(APawn* InstigatorPawn) override;

};
