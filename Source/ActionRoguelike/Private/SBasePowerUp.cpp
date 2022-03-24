// Fill out your copyright notice in the Description page of Project Settings.


#include "SBasePowerUp.h"

void ASBasePowerUp::BeginPlay()
{
    Super::BeginPlay();

    EnablePowerUp();
}

bool ASBasePowerUp::CanInteractWith(const APawn* InstigatorPawn) const
{
    return true;
}

void ASBasePowerUp::DoInteraction(APawn* InstigatorPawn)
{
    Super::DoInteraction(InstigatorPawn);

    UE_LOG(LogTemp, Log, TEXT("[%s] ASBasePowerUp::DoInteraction()"), *GetNameSafe(this));

    DisableAndCooldownPowerUp();
}

void ASBasePowerUp::EnablePowerUp()
{
    SetActiveState(true);
}

void ASBasePowerUp::DisableAndCooldownPowerUp()
{
    SetActiveState(false);

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ASBasePowerUp::EnablePowerUp, CooldownDurationSeconds);
}

void ASBasePowerUp::SetActiveState(const bool bIsActive)
{
    constexpr bool PropagateToChildren = true;
    RootComponent->SetVisibility(bIsActive, PropagateToChildren);

    SetActorEnableCollision(bIsActive);
}

