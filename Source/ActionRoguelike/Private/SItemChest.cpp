// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include <Particles/ParticleSystemComponent.h>

// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");
    RootComponent = BaseMesh;

    LidMesh = CreateDefaultSubobject<UStaticMeshComponent>("LidMesh");
    LidMesh->SetupAttachment(RootComponent);

    GoldMesh = CreateDefaultSubobject<UStaticMeshComponent>("GoldMesh");
    GoldMesh->SetupAttachment(RootComponent);

    GoldFlashingVFX = CreateDefaultSubobject<UParticleSystemComponent>("GoldFlashingVFX");
    GoldFlashingVFX->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
    LidMesh->SetRelativeRotation(FRotator(OpenedPitch, 0.0f, 0.0f));
}

