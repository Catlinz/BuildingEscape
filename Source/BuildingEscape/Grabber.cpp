// Copyright Catlin Zilinski 2016

#include "Grabber.h"

#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"

#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

// Gets and sets up the input component for grabbing and releasing.
void UGrabber::SetupInputComponent() {
	/// Look for the input component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found on %s"), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// line-trace (AKA ray-cast) out to reach distance.
	FHitResult HitResult;
	auto TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

// Gets a pointer to our physics handle component for grabbing.
void UGrabber::FindPhysicsHandleComponent() {
	/// Look for attached physics handle.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found on %s"), *GetOwner()->GetName());
	}
}

// Try and grab an object in front via line trace and physics handle.
void UGrabber::Grab() {
	/// Try and reach any actors with physics body collision channel set.
	auto HitResult = GetFirstPhysicsBodyInReach();

	/// If we hit something then attach a physics handle.
	auto ActorHit = HitResult.GetActor();
	auto ComponentToGrab = HitResult.GetComponent();

	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, 
			ActorHit->GetActorLocation(),
			true // allow rotation
		);
	}
}

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If physics handle is attached, move the object we're holding.
	if (PhysicsHandle->GetGrabbedComponent()) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	

}

const FVector UGrabber::GetReachLineEnd() {
	FVector EyeLocation;  FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	return EyeLocation + (EyeRotation.Vector() * GrabReachCm);
}

const FVector UGrabber::GetReachLineStart()
{
	FVector EyeLocation;  FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	return EyeLocation;
}
