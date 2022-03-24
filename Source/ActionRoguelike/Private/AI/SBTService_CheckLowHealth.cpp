// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckLowHealth.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>

#include "ActionRoguelike/Public/SAttributeComponent.h"

bool HasReachedLowHealthPercentage(const USAttributeComponent& AttributeComponent, const float LowHealthPercentage)
{
    const float HealthMax = AttributeComponent.GetHealthMax();
    const float Health = AttributeComponent.GetHealth();

    float CurrentHealthPercentage = 0.0f;
    if (HealthMax > 0.0f)
    {
        CurrentHealthPercentage = Health / HealthMax;
    }

    return CurrentHealthPercentage <= LowHealthPercentage;
}

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackBoardComponent = OwnerComp.GetBlackboardComponent();
    if (ensure(BlackBoardComponent != nullptr))
    {
        const AAIController* AiController = OwnerComp.GetAIOwner();
        if (ensure(AiController != nullptr))
        {
            const APawn* AiPawn = AiController->GetPawn();
            if (ensure(AiPawn != nullptr))
            {
                const USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(AiPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
                if (AttributeComponent != nullptr)
                {
                    BlackBoardComponent->SetValueAsBool(IsLowHealthKey.SelectedKeyName, HasReachedLowHealthPercentage(*AttributeComponent, LowHealthPercentage));
                }
            }
        }
    }
}
