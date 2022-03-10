// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"

#include <Components/StaticMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <PhysicsEngine/RadialForceComponent.h>

#include "ActionRoguelike/Public/SMagicProjectile.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    StaticMesh->SetSimulatePhysics(true);
    StaticMesh->SetCollisionProfileName("ExplosiveObject");
    RootComponent = StaticMesh;

    RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
    RadialForce->Radius = 1000.0f;
    RadialForce->ImpulseStrength = 2000.0f;
    RadialForce->bImpulseVelChange = true;
    RadialForce->SetupAttachment(RootComponent);

    ExplosionEffectSpawnPoint = CreateDefaultSubobject<USceneComponent>("ExplosionEffectSpawnPoint");
    ExplosionEffectSpawnPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASExplosiveBarrel::OnStaticMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor->IsA<ASMagicProjectile>())
    {
        UE_LOG(LogTemp, Log, TEXT("[ASExplosiveBarrel] Actor %s begins to overlap StaticMesh at location %s"), *GetNameSafe(OtherActor), *SweepResult.ImpactPoint.ToString());
        Explode();
    }
}

void ASExplosiveBarrel::OnStaticMeshHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->IsA<ASMagicProjectile>())
    {
        UE_LOG(LogTemp, Log, TEXT("[ASExplosiveBarrel] Actor %s hit StaticMesh at location %s"), *GetNameSafe(OtherActor), *Hit.ImpactPoint.ToString());
        Explode();
    }
}

// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASExplosiveBarrel::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ASExplosiveBarrel::OnStaticMeshBeginOverlap);
    StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnStaticMeshHit);
}

void ASExplosiveBarrel::Explode()
{
    RadialForce->FireImpulse();

    UWorld* world = GetWorld();

    FTransform SpawnTransform = FTransform::Identity;
    SpawnTransform.SetTranslation(ExplosionEffectSpawnPoint->GetComponentLocation());

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    world->SpawnActor<AActor>(ExplosionEffect, SpawnTransform, SpawnParams);

    world->DestroyActor(this);
}

