// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, AActor*, OwningPlayer, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASPlayerStateBase : public APlayerState
{
	GENERATED_BODY()
	
public:

    ASPlayerStateBase();

    UFUNCTION(BlueprintCallable)
    bool CanAffordCredits(const int32 Price) const;

    UFUNCTION(BlueprintCallable)
    int32 GetCredits() const;

    UFUNCTION(BlueprintCallable)
    int32 GetCreditsMax() const;

    UFUNCTION(BlueprintCallable)
    bool ApplyCreditsChange(int32 Delta);

    UPROPERTY(BlueprintAssignable)
    FOnCreditsChanged OnCreditsChanged;

private:

    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0"))
    int32 Credits;

    UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true", ClampMin = "0"))
    int32 CreditsMax;

};
