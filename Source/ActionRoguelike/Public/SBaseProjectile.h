// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseProjectile.generated.h"

class UParticleSystemComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS(ABSTRACT)
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

    virtual void PreInitializeComponents() override;

///////////////////////////

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    float MovementSpeed = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    float Lifetime = InfiniteLifetime;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    bool bDestroyOnOverlap = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    bool bExplodeOnOverlap = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    bool bExplodeOnLifetimeEnd = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    bool bIgnoreInstigatorWhenMoving = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BaseProjectile")
    bool bIgnoreInstigatorWhenOverlap = true;

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

    virtual bool IsOverlapValid(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    virtual void OnOverlapActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit);

    virtual void Explode();

    virtual void OnLifetimeTimerElapsed();

    virtual void DestroyProjectile();

private:
    // Only the base implementation needs to be an UFUNCTION, other derived classes just have to override this function without the UFUNCTION() macro
    UFUNCTION()
    void OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    static const float InfiniteLifetime/* = -1.0f*/;
    FTimerHandle TimerHandle_Lifetime;

};
