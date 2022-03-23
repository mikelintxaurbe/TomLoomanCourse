// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>
#include <Perception/PawnSensingComponent.h>

// Sets default values
ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC != nullptr)
    {
        UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
        BBComp->SetValueAsObject("TargetActor", Pawn);

        constexpr float DebugTextDuration = 4.0f;
        DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, DebugTextDuration);
    }
}

