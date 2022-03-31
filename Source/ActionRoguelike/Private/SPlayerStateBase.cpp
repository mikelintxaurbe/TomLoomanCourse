// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerStateBase.h"

namespace
{
    constexpr int32 CreditsMin = 0;
}

ASPlayerStateBase::ASPlayerStateBase()
{
    Credits = CreditsMin;
    CreditsMax = 1000;
}

bool ASPlayerStateBase::CanAffordCredits(const int32 Price) const
{
    return Credits >= Price;
}

int32 ASPlayerStateBase::GetCredits() const
{
    return Credits;
}

int32 ASPlayerStateBase::GetCreditsMax() const
{
    return CreditsMax;
}

bool ASPlayerStateBase::ApplyCreditsChange(int32 Delta)
{
    int32 PrevCredits = Credits;
    int32 PrevDelta = Delta;

    Credits = FMath::Clamp(Credits + Delta, CreditsMin, CreditsMax);
    Delta = Credits - PrevCredits;

    const bool Changed = Credits != PrevCredits;

    if (Changed)
    {
        UE_LOG(LogTemp, Log, TEXT("[%s] ASPlayerStateBase::ApplyCreditsChange(): Credits = %d -> %d | Delta = %d -> %d"), *GetPlayerName(), PrevCredits, Credits, PrevDelta, Delta);

        OnCreditsChanged.Broadcast(GetPawn(), Credits, Delta);
    }

    return Changed;
}
