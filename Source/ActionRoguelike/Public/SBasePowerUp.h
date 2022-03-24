// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseInteractive.h"
#include "SBasePowerUp.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASBasePowerUp : public ASBaseInteractive
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CooldownDurationSeconds = 10.0f;
	
protected:

    virtual void BeginPlay() override;

    virtual bool CanInteractWith(const APawn* InstigatorPawn) const override;

    virtual void DoInteraction(APawn* InstigatorPawn) override;

private:

    void EnablePowerUp();

    void DisableAndCooldownPowerUp();

    void SetActiveState(const bool bIsActive);

};
