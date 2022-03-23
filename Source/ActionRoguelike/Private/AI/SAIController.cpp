// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"

#include <BehaviorTree/BlackboardComponent.h>
#include <Kismet/GameplayStatics.h>

void ASAIController::BeginPlay()
{
    Super::BeginPlay();

    if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller.")))
    {
        RunBehaviorTree(BehaviorTree);
    }

    // Old way of setting TargetActor, now done from SAICharacter with UPawnSensingComponent
    /*constexpr int32 DefaultPlayerIndex = 0;
    APawn* MyPawn = UGameplayStatics::GetPlayerPawn(this, DefaultPlayerIndex);
    if (MyPawn != nullptr)
    {
        UBlackboardComponent* BlackBoardComponent = GetBlackboardComponent();
        BlackBoardComponent->SetValueAsVector("MoveToLocation", MyPawn->GetActorLocation());
        BlackBoardComponent->SetValueAsObject("TargetActor", MyPawn);
    }*/
}
