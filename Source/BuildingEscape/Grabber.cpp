// Copyright Catlin Zilinski 2016

#include "Grabber.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Public/DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point this tick
	FVector EyeLocation;
	FRotator EyeRotation;

	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *EyeLocation.ToString(), *EyeRotation.ToString());*/

	// Ray-cast out to reach distance
	FVector Direction = EyeRotation.Vector();
	FVector LineTraceEnd = EyeLocation + (Direction * GrabReachCm);

	DrawDebugLine(GetWorld(), EyeLocation, LineTraceEnd, FColor::Red, false, -1.0f, 0, 1.0f);

	// See what we hit.
}

