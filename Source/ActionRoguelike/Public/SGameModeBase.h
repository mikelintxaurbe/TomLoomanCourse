// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <EnvironmentQuery/EnvQueryTypes.h>
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UCurveFloat;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    TSubclassOf<AActor> MinionClass;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UEnvQuery* SpawnBotQuery;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UCurveFloat* DifficultyCurve;

    FTimerHandle TimerHandle_SpawnBots;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    float SpawnTimerInterval;

    UFUNCTION()
    void SpawnBotTimerElapsed();

    UFUNCTION()
    void OnSpawnBotQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

    UFUNCTION()
    void RespawnPlayerElapsed(AController* Controller);

protected:

    UPROPERTY(EditDefaultsOnly, Category = "GameMode | Scoring")
    int32 CreditsGainedOnBotKill;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode | PowerUps")
    UEnvQuery* SpawnPowerUpQuery;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode | PowerUps")
    TSubclassOf<AActor> HealthPotionClass;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode | PowerUps", Meta = (ClampMin = "0"))
    uint32 MaxHealthPotions = 5;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode | PowerUps")
    TSubclassOf<AActor> CoinClass;

    UPROPERTY(EditDefaultsOnly, Category = "GameMode | PowerUps", Meta = (ClampMin = "0"))
    uint32 MaxCoins = 5;

    void TryStartSpawnPowerUpQuery();

    UFUNCTION()
    void OnSpawnPowerUpQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

private:

    bool CanSpawnPowerUp() const;

    uint32 HealthPotionsPendingForSpawn = 0;

    uint32 CoinsPendingForSpawn = 0;

public:

    virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

    ASGameModeBase();

    virtual void StartPlay() override;

    UFUNCTION(Exec)
    void KillAll();

};
