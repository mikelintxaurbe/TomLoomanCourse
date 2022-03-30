// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    static USAttributeComponent* GetAttributes(AActor* FromActor);

    UFUNCTION(BlueprintCallable, Category = "Attributes", meta = (DisplayName = "IsAlive"))
    static bool IsActorAlive(AActor* Actor);

	// Sets default values for this component's properties
	USAttributeComponent();

protected:

    // Other possible attributes: HealthMax, Stamina, Strength

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetHealth, Category = "Attributes")
    float Health;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
    float HealthMax;

public:	

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    bool Kill(AActor* InstigatorActor);

    UFUNCTION(BlueprintCallable)
    bool IsAlive() const;

    float GetHealth() const;

    float GetHealthMax() const;

    UPROPERTY(BlueprintAssignable)
    FOnHealthChanged OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Attributes")
    bool ApplyHealthChange(AActor* InstigatorActor, float Delta);

private:

    UFUNCTION(BlueprintSetter)
    void SetHealth(float Value);

};
