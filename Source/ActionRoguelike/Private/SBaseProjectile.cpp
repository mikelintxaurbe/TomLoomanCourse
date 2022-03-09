// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"

#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Particles/ParticleSystemComponent.h>

const float ASBaseProjectile::InfiniteLifetime = -1.0f;

// Sets default values
ASBaseProjectile::ASBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
    SphereCollision->SetCollisionProfileName("ProjectileSolid");
    RootComponent = SphereCollision;

    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
    MovementComp->InitialSpeed = MovementSpeed;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
    MovementComp->ProjectileGravityScale = 0.0f;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetAutoActivate(true);
    EffectComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

    if (Lifetime != InfiniteLifetime)
    {
        if (Lifetime > 0.0f)
        {
            GetWorldTimerManager().SetTimer(TimerHandle_Lifetime, this, &ASBaseProjectile::OnLifetimeTimerElapsed, Lifetime);
        }
        else
        {
            OnLifetimeTimerElapsed();
        }
    }

    SphereCollision->IgnoreActorWhenMoving(GetInstigator(), true);
    SphereCollision->OnComponentHit.AddDynamic(this, &ASBaseProjectile::OnSphereCollisionHit);
}

// Called every frame
void ASBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBaseProjectile::OnSphereCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseProjectile::OnSphereCollisionHit()"), *GetNameSafe(this));
    UE_LOG(LogTemp, Log, TEXT("[%s]     - Hit actor: %s"), *GetNameSafe(this), *GetNameSafe(Hit.GetActor()));

    constexpr float DebugSphereRadius = 10.0f;
    constexpr float DebugSphereDuration = 2.0f;
    UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), DebugSphereRadius, 12.0f, FColor::Red, DebugSphereDuration);

    const FString DebugStringText = FString::Printf(TEXT("Hit actor: %s"), *GetNameSafe(Hit.GetActor()));
    constexpr float DebugTextDuration = 2.0f;
    UKismetSystemLibrary::DrawDebugString(this, GetActorLocation(), DebugStringText, nullptr, FColor::Red, DebugTextDuration);

    UWorld* World = GetWorld();

    const FTransform ActorTM = GetActorTransform();
    UGameplayStatics::SpawnEmitterAtLocation(World, ImpactVFX, ActorTM);
    UGameplayStatics::SpawnSoundAtLocation(World, ImpactSFX, ActorTM.GetLocation());

    if (bDestroyOnHit)
    {
        World->DestroyActor(this);
    }
}

void ASBaseProjectile::OnLifetimeTimerElapsed()
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseProjectile::OnLifetimeTimerElapsed()"), *GetNameSafe(this));

    GetWorld()->DestroyActor(this);
}

