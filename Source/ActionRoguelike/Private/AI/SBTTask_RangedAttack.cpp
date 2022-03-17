// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <GameFramework/Character.h>

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    AAIController* MyController = OwnerComp.GetAIOwner();
    if (ensure(MyController != nullptr))
    {
        ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
        if (MyPawn == nullptr)
        {
            return EBTNodeResult::Failed;
        }

        const FVector MuzzleLocation = MyPawn->GetMesh()->GetSocketLocation("Muzzle_01");

        UBlackboardComponent* BlackBoardComponent = OwnerComp.GetBlackboardComponent();
        AActor* TargetActor = Cast<AActor>(BlackBoardComponent->GetValueAsObject("TargetActor"));
        if (TargetActor == nullptr)
        {
            return EBTNodeResult::Failed;
        }

        // Same thing as UKismetMathLibrary::FindLookAtRotation()
        const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
        FRotator MuzzleRotation = Direction.Rotation();

        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Params.Instigator = MyPawn;

        AActor* NewProj = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, Params);

        return NewProj != nullptr ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
    }

    return EBTNodeResult::Failed;
}
