// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBasePowerUp.h"
#include "SCoin.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASBasePowerUp
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coin", Meta = (ClampMin = "0"))
    int32 CreditsGained = 5;

protected:

    virtual void DoInteraction(APawn* InstigatorPawn) override;

};
