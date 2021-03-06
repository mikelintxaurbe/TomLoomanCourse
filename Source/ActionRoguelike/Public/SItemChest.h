// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UParticleSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

    UPROPERTY(VisibleAnywhere)
    float ClosedPitch = 0.0f;

    UPROPERTY(VisibleAnywhere)
    float OpenedPitch = 110.0f;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* BaseMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* LidMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* GoldMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UParticleSystemComponent* GoldFlashingVFX;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    void Interact_Implementation(APawn* InstigatorPawn) override;

};
