// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/KismetSystemLibrary.h>

#include "ActionRoguelike/Public/SInteractionComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

    InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0;
	ControlRot.Roll = 0;

	// UE4 direction axis
	//	- x: forward (red)
	//	- y: right (green)
	//	- z: up (blue)
	const FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRot);

	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack()
{
    PerformAbility(PrimaryAttackData);
}

void ASCharacter::SecondaryAbility()
{
    PerformAbility(SecondaryAbilityData);
}

void ASCharacter::Jump()
{
    // ACharacter already implements jumping for us
    Super::Jump();
}

void ASCharacter::PrimaryInteract()
{
    InteractionComp->PrimaryInteract();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryAttack);
    PlayerInputComponent->BindAction("SecondaryAbility", EInputEvent::IE_Pressed, this, &ASCharacter::SecondaryAbility);
    PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ASCharacter::Jump);
    PlayerInputComponent->BindAction("PrimaryInteract", EInputEvent::IE_Pressed, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::PerformAbility(const FAbilityData& AbilityData)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASCharacter::PerformAbility()"), *GetNameSafe(this));

    if (ensure(AbilityData.CharacterActionAnim))
    {
        PlayAnimMontage(AbilityData.CharacterActionAnim);

        if (AbilityData.ProjectileSpawnDelay > 0.0f)
        {
            FTimerDelegate TimerCallback;
            TimerCallback.BindLambda([this, &AbilityData]()
            {
                PerformAbility_TimerElapsed(AbilityData);
            });

            constexpr bool LoopTimer = false;
            GetWorldTimerManager().SetTimer(PerformAbility_TimerHandle, TimerCallback, AbilityData.ProjectileSpawnDelay, LoopTimer);
        }
        else
        {
            PerformAbility_TimerElapsed(AbilityData);
        }
    }
}

FVector GetProjectileSpawnPosition(const ACharacter* Instigator, const FAbilityData& AbilityData)
{
    check(Instigator != nullptr);

    if (AbilityData.ProjectileSpawnLocationSocketName.IsNone())
    {
        return Instigator->GetActorLocation();
    }
    
    return Instigator->GetMesh()->GetSocketLocation(AbilityData.ProjectileSpawnLocationSocketName);
}

FVector GetProjectileTargetPosition(const AActor* Instigator, const AController* InstigatorController, const float LinecastDistance)
{
    check(Instigator != nullptr);
    check(InstigatorController != nullptr);

    FVector EyePosition;
    FRotator EyeRotation;
    InstigatorController->GetPlayerViewPoint(EyePosition, EyeRotation);

    FVector LinecastEnd = EyePosition + (LinecastDistance * EyeRotation.Vector());

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);

    FCollisionQueryParams& QueryParams = FCollisionQueryParams::DefaultQueryParam;
    QueryParams.AddIgnoredActor(Instigator);

    FHitResult HitResult;
    const bool LineTraceSuccess = Instigator->GetWorld()->LineTraceSingleByObjectType(HitResult, EyePosition, LinecastEnd, ObjectQueryParams, QueryParams);

    const FLinearColor DebugDrawingColor = LineTraceSuccess ? FLinearColor::Green : FLinearColor::Red;
    constexpr float DebugDrawingDuration = 2.0f;

    constexpr float DebugLineThickness = 2.0f;
    UKismetSystemLibrary::DrawDebugLine(Instigator, EyePosition, LineTraceSuccess ? HitResult.ImpactPoint : LinecastEnd, DebugDrawingColor, DebugDrawingDuration, DebugLineThickness);

    if (LineTraceSuccess)
    {
        constexpr float DebugSphereRadius = 15.0f;
        UKismetSystemLibrary::DrawDebugSphere(Instigator, HitResult.ImpactPoint, DebugSphereRadius, 12, DebugDrawingColor, DebugDrawingDuration);

        return HitResult.ImpactPoint;
    }

    return LinecastEnd;
}

void ASCharacter::PerformAbility_TimerElapsed(const FAbilityData& AbilityData)
{
    const FVector ProjectileSpawnLocation = GetProjectileSpawnPosition(this, AbilityData);

    const FVector ProjectileTarget = GetProjectileTargetPosition(this, GetController(), AbilityData.ProjectileTargetLinecastDistance);

    const FRotator TargetFacingRotator = UKismetMathLibrary::FindLookAtRotation(ProjectileSpawnLocation, ProjectileTarget);
    FTransform ProjectileSpawnTM = FTransform(TargetFacingRotator, ProjectileSpawnLocation);

    FActorSpawnParameters ProjectileSpawnParams;
    ProjectileSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    ProjectileSpawnParams.Instigator = this;

    GetWorld()->SpawnActor<AActor>(AbilityData.ProjectileClass, ProjectileSpawnTM, ProjectileSpawnParams);
}

