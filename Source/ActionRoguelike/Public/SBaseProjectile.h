// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class UParticleSystemComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASBaseProjectile();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

///////////////////////////

public:
    UPROPERTY(EditAnywhere, Category = "BaseProjectile")
    float MovementSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, Category = "BaseProjectile")
    float Lifetime = InfiniteLifetime;

    UPROPERTY(EditAnywhere, Category = "BaseProjectile")
    bool bDestroyOnHit = true;

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    USphereComponent* SphereCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    UProjectileMovementComponent* MovementComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    UParticleSystemComponent* EffectComp;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    UParticleSystem* ImpactVFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    USoundBase* ImpactSFX;

    // Only the base implementation needs to be an UFUNCTION, other derived classes just have to override this function without the UFUNCTION() macro
    UFUNCTION()
    virtual void OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void OnLifetimeTimerElapsed();

private:
    static const float InfiniteLifetime/* = -1.0f*/;
    FTimerHandle TimerHandle_Lifetime;

};
