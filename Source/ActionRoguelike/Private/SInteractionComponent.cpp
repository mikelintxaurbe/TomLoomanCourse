// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include <Kismet/KismetSystemLibrary.h>

#include "ActionRoguelike/Public/SGameplayInterface.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
    AActor* Owner = GetOwner();

    FVector EyeLocation;
    FRotator EyeRotation;
    Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

    const FVector RaycastStart = EyeLocation;
    const FVector RaycastEnd = RaycastStart + EyeRotation.Vector() * RaycastDistance;

    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    FCollisionShape Sphere;
    Sphere.SetSphere(RaycastSphereRadius);

    /*FHitResult HitResult;
    const bool Result = GetWorld()->LineTraceSingleByObjectType(HitResult, RaycastStart, RaycastEnd, ObjectQueryParams);*/

    TArray<FHitResult> HitResults;
    const bool Result = GetWorld()->SweepMultiByObjectType(HitResults, RaycastStart, RaycastEnd, FQuat::Identity, ObjectQueryParams, Sphere);

    FLinearColor DebugLineColor = Result ? FLinearColor::Yellow : FLinearColor::Red;
    constexpr float DebugDrawingDuration = 2.0f;

    for (const auto Hit : HitResults)
    {
        bool GameplayInterfaceFound = false;

        if (AActor* HitActor = Hit.GetActor())
        {
            if (HitActor->Implements<USGameplayInterface>())
            {
                GameplayInterfaceFound = true;
                DebugLineColor = FLinearColor::Green;
            }
        }

        const FLinearColor DebugSphereColor = GameplayInterfaceFound ? FLinearColor::Green : FLinearColor::Yellow;
        
        UKismetSystemLibrary::DrawDebugSphere(this, Hit.ImpactPoint, RaycastSphereRadius, 12, DebugSphereColor, DebugDrawingDuration);

        if (GameplayInterfaceFound)
        {
            ISGameplayInterface::Execute_Interact(Hit.GetActor(), Cast<APawn>(Owner));
            break;
        }
    }

    constexpr float DebugLineThickness = 2.0f;
    UKismetSystemLibrary::DrawDebugLine(this, RaycastStart, RaycastEnd, DebugLineColor, DebugDrawingDuration, DebugLineThickness);
}

