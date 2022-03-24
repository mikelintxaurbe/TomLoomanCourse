// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_HealToMaxHealth.h"

#include <AIController.h>

#include "ActionRoguelike/Public/SAttributeComponent.h"

EBTNodeResult::Type USBTTask_HealToMaxHealth::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AiController = OwnerComp.GetAIOwner();
    if (ensure(AiController != nullptr))
    {
        APawn* AiPawn = AiController->GetPawn();
        if (AiPawn != nullptr)
        {
            USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(AiPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
            if (AttributeComponent != nullptr)
            {
                AttributeComponent->ApplyHealthChange(AttributeComponent->GetHealthMax());
                return EBTNodeResult::Succeeded;
            }
        }
    }

    return EBTNodeResult::Failed;
}
