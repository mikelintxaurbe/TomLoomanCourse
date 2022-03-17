// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"

#include <Components/AudioComponent.h>
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
    SphereCollision->SetCollisionProfileName("Projectile");
    RootComponent = SphereCollision;

    MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
    MovementComp->InitialSpeed = MovementSpeed;
    MovementComp->bRotationFollowsVelocity = true;
    MovementComp->bInitialVelocityInLocalSpace = true;
    MovementComp->ProjectileGravityScale = 0.0f;

    EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
    EffectComp->SetAutoActivate(true);
    EffectComp->SetupAttachment(RootComponent);

    AudioComp = CreateDefaultSubobject<UAudioComponent>("AudioComp");
    AudioComp->SetAutoActivate(true);
    AudioComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();

    SphereCollision->IgnoreActorWhenMoving(GetInstigator(), bIgnoreInstigatorWhenMoving);
    SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ASBaseProjectile::OnSphereCollisionBeginOverlap);

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
}

// Called every frame
void ASBaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBaseProjectile::PreInitializeComponents()
{
    // Need this for applying any value that's not the default one
    MovementComp->InitialSpeed = MovementSpeed;
}

bool ASBaseProjectile::IsOverlapValid(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    const AActor* MyInstigator = Cast<AActor>(GetInstigator());

    if (bIgnoreInstigatorWhenOverlap && OtherActor == MyInstigator)
    {
        return false;
    }

    if (bIgnoreProjectilesOfSameInstigator && OtherActor->IsA<ASBaseProjectile>())
    {
        const AActor* OtherActorInstigator = Cast<AActor>(OtherActor->GetInstigator());

        if (OtherActorInstigator == MyInstigator)
        {
            return false;
        }
    }

    return true;
}

void ASBaseProjectile::OnOverlapActor(AActor* OtherActor, UPrimitiveComponent* OtherComp, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseProjectile::OnOverlapActor()"), *GetNameSafe(this));
    UE_LOG(LogTemp, Log, TEXT("[%s]     - Hit actor: %s"), *GetNameSafe(this), *GetNameSafe(OtherActor));

    constexpr float DebugSphereRadius = 10.0f;
    constexpr float DebugSphereDuration = 2.0f;
    UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), DebugSphereRadius, 12.0f, FColor::Red, DebugSphereDuration);

    const FString DebugStringText = FString::Printf(TEXT("Hit actor: %s"), *GetNameSafe(OtherActor));
    constexpr float DebugTextDuration = 2.0f;
    UKismetSystemLibrary::DrawDebugString(this, GetActorLocation(), DebugStringText, nullptr, FColor::Red, DebugTextDuration);
}

void ASBaseProjectile::Explode()
{
    UWorld* World = GetWorld();

    const FTransform ActorTM = GetActorTransform();
    UGameplayStatics::SpawnEmitterAtLocation(World, ImpactVFX, ActorTM);
    UGameplayStatics::PlaySoundAtLocation(World, ImpactSFX, ActorTM.GetLocation()); // better than SpawnSoundAtLocation() because this doesn't create an UAudioComponent in the world
}

void ASBaseProjectile::OnLifetimeTimerElapsed()
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseProjectile::OnLifetimeTimerElapsed()"), *GetNameSafe(this));

    if (bExplodeOnLifetimeEnd)
    {
        Explode();
    }

    DestroyProjectile();
}

void ASBaseProjectile::DestroyProjectile()
{
    GetWorld()->DestroyActor(this);
}

void ASBaseProjectile::OnSphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("[%s] ASBaseProjectile::OnSphereCollisionBeginOverlap()"), *GetNameSafe(this));

    if (IsOverlapValid(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult) == false)
    {
        return;
    }

    OnOverlapActor(OtherActor, OtherComp, SweepResult);

    if (bExplodeOnOverlap)
    {
        Explode();
    }

    if (bDestroyOnOverlap)
    {
        DestroyProjectile();
    }
}

