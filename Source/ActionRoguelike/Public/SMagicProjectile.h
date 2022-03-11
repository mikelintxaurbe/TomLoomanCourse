// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/Public/SBaseProjectile.h"
#include "GameFramework/Actor.h"
#include "SMagicProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASBaseProjectile
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MagicProjectile")
    float Damage = -20.0f;

protected:

    virtual void OnOverlapActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit) override;

};
