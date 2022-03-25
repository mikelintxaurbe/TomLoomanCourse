// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class USAttributeComponent;
class UPawnSensingComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, Category = "Effects | HitFlash")
    FName HitFlashTimeParamName;

    UPROPERTY(EditAnywhere, Category = "Effects | HitFlash")
    FName HitFlashDurationParamName;

    UPROPERTY(EditAnywhere, Category = "Effects | HitFlash")
    float HitFlashDurationSeconds = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Effects | HitFlash")
    FName HitFlashColorParamName;

    UPROPERTY(EditAnywhere, Category = "Effects | HitFlash")
    FLinearColor HitFlashDamagedColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, Category = "Effects | HitFlash")
    FLinearColor HitFlashHealedColor = FLinearColor::Green;

	// Sets default values for this character's properties
	ASAICharacter();

    virtual void PostInitializeComponents() override;

protected:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UPawnSensingComponent* PawnSensingComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USAttributeComponent* AttributeComp;

    UFUNCTION()
    void OnPawnSeen(APawn* Pawn);

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

    void SetTargetActor(AActor* NewTarget);

};
