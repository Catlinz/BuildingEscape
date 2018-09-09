// Copyright Catlin Zilinski 2016

#include "OpenDoor.h"

#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate!"), *Owner->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	OnCloseRequest.Broadcast();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// If there is enough mass on pressure plate, then open the door (if not already opened).
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}

const float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	if (!PressurePlate) { return 0.0f; }
	/// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	
	/// Iterate through them adding their masses.
	float TotalMass = 0.f;
	for (const auto* OverlappingActor : OverlappingActors) {
		auto Component = OverlappingActor->FindComponentByClass<UPrimitiveComponent>();
		if (Component != nullptr) {
			TotalMass += Component->GetMass();
		}
	}

	return TotalMass;
}

