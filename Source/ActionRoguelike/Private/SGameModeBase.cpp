// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include <EngineUtils.h>
#include <EnvironmentQuery/EnvQueryManager.h>

#include "ActionRoguelike/Public/AI/SAICharacter.h"
#include "ActionRoguelike/Public/SAttributeComponent.h"

ASGameModeBase::ASGameModeBase()
{
    SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
    Super::StartPlay();

    constexpr bool bDoesLoop = true;
    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, bDoesLoop);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
    if (ensure(QueryInstance != nullptr))
    {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
    }
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus != EEnvQueryStatus::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
        return;
    }

    int32 NrOfAliveBots = 0;
    for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        ASAICharacter* Bot = *It;

        USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
        if (AttributeComp != nullptr && AttributeComp->IsAlive())
        {
            ++NrOfAliveBots;
        }
    }

    float MaxBotCount = 10.0f;

    if (DifficultyCurve != nullptr)
    {
        MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
    }

    if (static_cast<float>(NrOfAliveBots) >= MaxBotCount)
    {
        return;
    }

    //TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

    TArray<FVector> Locations;
    QueryInstance->GetQueryResultsAsLocations(Locations);

    //if (Locations.Num() > 0)
    if (Locations.IsValidIndex(0))
    {
        GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);
    }
}

