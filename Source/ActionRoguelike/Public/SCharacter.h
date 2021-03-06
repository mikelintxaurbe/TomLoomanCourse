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

    UPROPERTY(EditAnywhere)
    UParticleSystem* ProjectileSpawnVFX = nullptr;

    UPROPERTY(EditAnywhere, Meta=(ClampMin = "0"))
    float ProjectileTargetLinecastDistance = 100000.0f;

    UPROPERTY(EditAnywhere, Meta = (ClampMin = "0"))
    float ProjectileTargetLinecastSphereRadius = 20.0f;
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

    UFUNCTION()
    void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

    FVector GetPawnViewLocation() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void PostInitializeComponents() override;

    UFUNCTION(Exec)
    void HealSelf(float Ammount = 100.0f);

private:
    void PerformAbility(const FAbilityData& AbilityData);
    void PerformAbility_TimerElapsed(const FAbilityData& AbilityData);

    FTimerHandle PerformAbility_TimerHandle;

};
