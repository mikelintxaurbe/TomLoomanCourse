// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SBaseInteractive.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASBaseInteractive
{
	GENERATED_BODY()
	
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float HealthRestored = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float InactiveDurationSeconds = 10.0f;

protected:

    bool CanInteractWith(const APawn* InstigatorPawn) const override;

    void DoInteraction(APawn* InstigatorPawn) override;

private:

    enum class EActiveState
    {
        Active,
        Inactive
    };

    void SetActiveState(const EActiveState NewActiveState);

    EActiveState CurrentActiveState;

};
