// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class URadialForceComponent;
class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ExplosionEffect;

protected:

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* StaticMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    URadialForceComponent* RadialForce;

    UPROPERTY(EditAnywhere)
    USceneComponent* ExplosionEffectSpawnPoint;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    /*UFUNCTION()
    void OnStaticMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);*/

    UFUNCTION()
    void OnStaticMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    virtual void PostInitializeComponents() override;

private:

    void Explode();

};
