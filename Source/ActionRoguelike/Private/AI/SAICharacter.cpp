// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>
#include <Perception/PawnSensingComponent.h>

#include "ActionRoguelike/Public/SAttributeComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

    AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
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

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    if (Delta == 0.0f)
    {
        return;
    }

    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    SkeletalMesh->SetScalarParameterValueOnMaterials("TimeAtHit", GetWorld()->TimeSeconds);
    SkeletalMesh->SetScalarParameterValueOnMaterials("FlashDurationSeconds", HitFlashDurationSeconds);

    if (Delta > 0.0f)
    {
        SkeletalMesh->SetVectorParameterValueOnMaterials("FlashColor", FVector{ HitFlashHealedColor });
    }
    else
    {
        SkeletalMesh->SetVectorParameterValueOnMaterials("FlashColor", FVector{ HitFlashDamagedColor });
    }

    if (NewHealth <= 0.0f && Delta < 0.0f)
    {
        SetActorEnableCollision(false);

        AAIController* AIC = Cast<AAIController>(GetController());
        AIC->UnPossess();

        // Death animation is managed in the Animation Blueprint
    }
}

