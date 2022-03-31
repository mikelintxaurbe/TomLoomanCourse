// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"

#include "ActionRoguelike/Public/SPlayerStateBase.h"

void ASCoin::DoInteraction(APawn* InstigatorPawn)
{
    Super::DoInteraction(InstigatorPawn);

    ASPlayerStateBase* PlayerStateBase = InstigatorPawn->GetPlayerState<ASPlayerStateBase>();
    if (PlayerStateBase != nullptr)
    {
        PlayerStateBase->ApplyCreditsChange(CreditsGained);
    }
}
