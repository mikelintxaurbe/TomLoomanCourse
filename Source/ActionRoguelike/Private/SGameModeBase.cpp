// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include <DrawDebugHelpers.h>
#include <EngineUtils.h>
#include <EnvironmentQuery/EnvQueryManager.h>

#include "ActionRoguelike/Public/AI/SAICharacter.h"
#include "ActionRoguelike/Public/SAttributeComponent.h"
#include "ActionRoguelike/Public/SCharacter.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

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

void ASGameModeBase::KillAll()
{
    for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        ASAICharacter* Bot = *It;

        USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
        if (AttributeComp != nullptr && AttributeComp->IsAlive())
        {
            AttributeComp->Kill(this);  // @fixme: pass in player? for kill credit
        }
    }
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
    if (!CVarSpawnBots.GetValueOnGameThread())
    {
        UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::OnQueryCompleted] Bot spawning disabled via cvar 'CVarSpawnBots'."));
        return;
    }

    int32 NrOfAliveBots = 0;
    for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
    {
        ASAICharacter* Bot = *It;

        USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
        if (AttributeComp != nullptr && AttributeComp->IsAlive())
        {
            ++NrOfAliveBots;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::OnQueryCompleted] Found %i alive bots."), NrOfAliveBots);

    float MaxBotCount = 10.0f;

    if (DifficultyCurve != nullptr)
    {
        MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
    }

    if (static_cast<float>(NrOfAliveBots) >= MaxBotCount)
    {
        UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::OnQueryCompleted] At maximum bot capacity. Skipping bot spawn."));
        return;
    }

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
        UE_LOG(LogTemp, Warning, TEXT("[ASGameModeBase::OnQueryCompleted] Spawn bot EQS Query Failed!"));
        return;
    }

    //TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();

    TArray<FVector> Locations;
    QueryInstance->GetQueryResultsAsLocations(Locations);

    //if (Locations.Num() > 0)
    if (Locations.IsValidIndex(0))
    {
        GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator);

        constexpr float DebugSphereRadius = 50.0f;
        constexpr float DebugSphereLifetime = 60.0f;
        DrawDebugSphere(GetWorld(), Locations[0], DebugSphereRadius, 20.0f, FColor::Blue, false, DebugSphereLifetime);
    }
}

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
    if (ensure(Controller != nullptr))
    {
        Controller->UnPossess();

        RestartPlayer(Controller);
    }
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
    ASCharacter* Player = Cast<ASCharacter>(VictimActor);
    if (Player != nullptr)
    {
        FTimerHandle TimerHandle_RespawnDelay;

        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());

        constexpr float RespawnDelay = 2.0f;
        constexpr bool bDoesLoop = false;
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, bDoesLoop);
    }

    UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::OnActorKilled] Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

