// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SBaseInteractive.generated.h"

class USphereComponent;

UCLASS(ABSTRACT)
class ACTIONROGUELIKE_API ASBaseInteractive : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	

    // Sets default values for this actor's properties
    ASBaseInteractive();
	
protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* StaticMeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* InteractiveSphereCollision;

    virtual bool CanInteractWith(const APawn* InstigatorPawn) const;

    virtual void DoInteraction(APawn* InstigatorPawn);

private:

    void Interact_Implementation(APawn* InstigatorPawn) override;

};
