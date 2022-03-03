// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosionEffect.generated.h"

class UAudioComponent;
class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplosionEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosionEffect();

protected:

    UPROPERTY(VisibleAnywhere)
    UParticleSystemComponent* VFX;

    UPROPERTY(VisibleAnywhere)
    UAudioComponent* SFX;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
