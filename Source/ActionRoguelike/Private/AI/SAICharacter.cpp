// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include <AIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BrainComponent.h>
#include <Components/CapsuleComponent.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Perception/PawnSensingComponent.h>

#include "ActionRoguelike/Public/SAttributeComponent.h"
#include "ActionRoguelike/Public/SWorldUserWidget.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
    PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

    AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
    GetMesh()->SetGenerateOverlapEvents(true);

    HitFlashTimeParamName = "TimeAtHit";
    HitFlashDurationParamName = "FlashDurationSeconds";
    HitFlashColorParamName = "FlashColor";
}

void ASAICharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

    AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
    SetTargetActor(Pawn);

    constexpr float DebugTextDuration = 4.0f;
    DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, DebugTextDuration);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
    USkeletalMeshComponent* SkeletalMesh = GetMesh();
    SkeletalMesh->SetScalarParameterValueOnMaterials(HitFlashTimeParamName, GetWorld()->TimeSeconds);
    SkeletalMesh->SetScalarParameterValueOnMaterials(HitFlashDurationParamName, HitFlashDurationSeconds);

    if (Delta >= 0.0f)
    {
        SkeletalMesh->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector{ HitFlashHealedColor });
    }
    else
    {
        SkeletalMesh->SetVectorParameterValueOnMaterials(HitFlashColorParamName, FVector{ HitFlashDamagedColor });

        if (InstigatorActor != this)
        {
            SetTargetActor(InstigatorActor);
        }

        if (ActiveHealthBar == nullptr)
        {
            ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
            if (ActiveHealthBar != nullptr)
            {
                constexpr float OffsetHeight = 70.0f;

                ActiveHealthBar->AttachedActor = this;
                ActiveHealthBar->WorldOffset = FVector(0.0f, 0.0f, OffsetHeight);

                ActiveHealthBar->AddToViewport();
            }
        }
    }

    if (NewHealth <= 0.0f && Delta < 0.0f)
    {
        SetActorEnableCollision(false);

        AAIController* AIC = Cast<AAIController>(GetController());
        if (AIC != nullptr)
        {
            // stop BT
            //AIC->UnPossess();
            AIC->GetBrainComponent()->StopLogic("Killed");

            // ragdoll
            //GetMesh()->SetAllBodiesSimulatePhysics(true);
            //GetMesh()->SetCollisionProfileName("Ragdoll");
            // --> Death animation is managed in the Animation Blueprint <--

            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            GetCharacterMovement()->DisableMovement();

            // set lifespan
            constexpr float TimeToDie = 10.0f;
            SetLifeSpan(TimeToDie);
        }
    }
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
    AAIController* AIC = Cast<AAIController>(GetController());
    if (AIC != nullptr)
    {
        AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
    }
}

