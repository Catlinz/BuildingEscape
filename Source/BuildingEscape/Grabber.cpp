// Copyright Catlin Zilinski 2016

#include "Grabber.h"

#include "Engine/World.h"
#include "Engine/Public/DrawDebugHelpers.h"

#include "GameFramework/PlayerController.h"

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

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent() {
	/// Look for the input component (only appears at runtime)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Found input compoonent"));

		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("InputComponent not found on %s"), *GetOwner()->GetName())
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get player view point this tick
	FVector EyeLocation;
	FRotator EyeRotation;

	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	/// Ray-cast out to reach distance
	FVector Direction = EyeRotation.Vector();
	FVector LineTraceEnd = EyeLocation + (Direction * GrabReachCm);

	/// See what we hit.
	FHitResult HitResult;

	// Setup query parameters.
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		EyeLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorHit->GetName());
	}

	return HitResult;
}

void UGrabber::FindPhysicsHandleComponent() {
	/// Look for attached physics handle.
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// Physics handle is found.
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Physics handle not found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));

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
			true);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab key release"));

	PhysicsHandle->ReleaseComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If physics handle is attached, move the object we're holding.
	if (PhysicsHandle->GetGrabbedComponent()) {

		/// Get player view point this tick
		FVector EyeLocation;
		FRotator EyeRotation;

		GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		/// Ray-cast out to reach distance
		FVector Direction = EyeRotation.Vector();
		FVector LineTraceEnd = EyeLocation + (Direction * GrabReachCm);

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	

}

