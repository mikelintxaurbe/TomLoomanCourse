// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include <DrawDebugHelpers.h>
#include <EngineUtils.h>
#include <EnvironmentQuery/EnvQueryManager.h>

#include "ActionRoguelike/Public/AI/SAICharacter.h"
#include "ActionRoguelike/Public/SAttributeComponent.h"
#include "ActionRoguelike/Public/SCharacter.h"
#include "ActionRoguelike/Public/SPlayerStateBase.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
    SpawnTimerInterval = 2.0f;

    CreditsGainedOnBotKill = 10;
}

void ASGameModeBase::StartPlay()
{
    Super::StartPlay();

    constexpr bool bDoesLoop = true;
    GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, bDoesLoop);

    HealthPotionsPendingForSpawn = MaxHealthPotions;
    CoinsPendingForSpawn = MaxCoins;

    TryStartSpawnPowerUpQuery();
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
        UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::SpawnBotTimerElapsed] Bot spawning disabled via cvar 'CVarSpawnBots'."));
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

    UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::SpawnBotTimerElapsed] Found %i alive bots."), NrOfAliveBots);

    float MaxBotCount = 10.0f;

    if (DifficultyCurve != nullptr)
    {
        MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
    }

    if (static_cast<float>(NrOfAliveBots) >= MaxBotCount)
    {
        UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::SpawnBotTimerElapsed] At maximum bot capacity. Skipping bot spawn."));
        return;
    }

    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
    if (ensure(QueryInstance != nullptr))
    {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryCompleted);
    }
}

void ASGameModeBase::OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus != EEnvQueryStatus::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ASGameModeBase::OnSpawnBotQueryCompleted] Spawn bot EQS Query Failed!"));
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
    UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::OnActorKilled] Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));

    ASCharacter* PlayerVictim = Cast<ASCharacter>(VictimActor);
    if (PlayerVictim != nullptr)
    {
        FTimerHandle TimerHandle_RespawnDelay;

        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerVictim->GetController());

        constexpr float RespawnDelay = 2.0f;
        constexpr bool bDoesLoop = false;
        GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, bDoesLoop);
    }

    ASAICharacter* BotVictim = Cast<ASAICharacter>(VictimActor);
    if (BotVictim != nullptr)
    {
        ASCharacter* PlayerKiller = Cast<ASCharacter>(Killer);
        if (PlayerKiller != nullptr)
        {
            ASPlayerStateBase* PlayerStateBase = PlayerKiller->GetPlayerState<ASPlayerStateBase>();
            if (PlayerStateBase != nullptr)
            {
                PlayerStateBase->ApplyCreditsChange(CreditsGainedOnBotKill);
            }
        }
    }
}

void ASGameModeBase::TryStartSpawnPowerUpQuery()
{
    if (CanSpawnPowerUp() == false)
    {
        UE_LOG(LogTemp, Log, TEXT("[ASGameModeBase::StartSpawnPowerUpQuery] At maximum health potions and coins. Skipping power-up spawn."));
        return;
    }

    UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerUpQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
    if (QueryInstance != nullptr)
    {
        QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnPowerUpQueryCompleted);
    }
}

void ASGameModeBase::OnSpawnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
    if (QueryStatus != EEnvQueryStatus::Success)
    {
        UE_LOG(LogTemp, Warning, TEXT("[ASGameModeBase::OnSpawnPowerUpQueryCompleted] Spawn power up EQS Query Failed!"));
        return;
    }

    TArray<FVector> Locations;
    QueryInstance->GetQueryResultsAsLocations(Locations);

    constexpr uint32 SpawnLocationIndex = 0;
    if (Locations.IsValidIndex(SpawnLocationIndex))
    {
        TSubclassOf<AActor> PowerUpClass;

        if (HealthPotionsPendingForSpawn >= CoinsPendingForSpawn)
        {
            PowerUpClass = HealthPotionClass;
            --HealthPotionsPendingForSpawn;
        }
        else
        {
            PowerUpClass = CoinClass;
            --CoinsPendingForSpawn;
        }

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AActor* SpawnedPowerUp = GetWorld()->SpawnActor<AActor>(PowerUpClass, Locations[SpawnLocationIndex], FRotator::ZeroRotator, SpawnParams);

        UE_LOG(LogTemp, Warning, TEXT("[ASGameModeBase::OnSpawnPowerUpQueryCompleted] %s spawned at world-position %s"), *GetNameSafe(SpawnedPowerUp), *(Locations[SpawnLocationIndex].ToString()));

        TryStartSpawnPowerUpQuery();
    }
}

bool ASGameModeBase::CanSpawnPowerUp() const
{
    return HealthPotionsPendingForSpawn > 0 || CoinsPendingForSpawn > 0;
}

