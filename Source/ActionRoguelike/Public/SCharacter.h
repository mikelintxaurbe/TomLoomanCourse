// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USAttributeComponent;
class USInteractionComponent;
class USpringArmComponent;

USTRUCT()
struct FAbilityData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    UAnimMontage* CharacterActionAnim;

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ProjectileClass;

    UPROPERTY(EditAnywhere)
    FName ProjectileSpawnLocationSocketName;

    UPROPERTY(EditAnywhere, Meta=(ClampMin = "0"))
    float ProjectileSpawnDelay = 0.0f;

    UPROPERTY(EditAnywhere, Meta=(ClampMin = "0"))
    float ProjectileTargetLinecastDistance = 100000.0f;
};

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:

    UPROPERTY(EditAnywhere, Category = "Abilities")
    FAbilityData PrimaryAttackData;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    FAbilityData PrimaryAbilityData;

    UPROPERTY(EditAnywhere, Category = "Abilities")
    FAbilityData SecondaryAbilityData;

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere)
    USInteractionComponent* InteractionComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USAttributeComponent* AttributeComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
    void PrimaryAbility();
    void SecondaryAbility();

    void Jump();

    void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void PerformAbility(const FAbilityData& AbilityData);
    void PerformAbility_TimerElapsed(const FAbilityData& AbilityData);

    FTimerHandle PerformAbility_TimerHandle;

};
