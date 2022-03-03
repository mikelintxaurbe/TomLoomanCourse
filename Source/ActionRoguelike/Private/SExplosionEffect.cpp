// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosionEffect.h"

#include <Components/AudioComponent.h>
#include <Particles/ParticleSystemComponent.h>

// Sets default values
ASExplosionEffect::ASExplosionEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    VFX = CreateDefaultSubobject<UParticleSystemComponent>("VFX");
    VFX->SetupAttachment(RootComponent);

    SFX = CreateDefaultSubobject<UAudioComponent>("SFX");
    SFX->Play();
    SFX->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASExplosionEffect::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASExplosionEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

