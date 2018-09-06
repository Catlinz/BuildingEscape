// Copyright Catlin Zilinski 2016

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ActorThatOpens = nullptr;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0, OpenAngle, 0));
	LastDoorOpenTimeSec = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0, 0, 0));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume every frame
	if (ActorThatOpens != nullptr && PressurePlate != nullptr && PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		 // If the ActorThatOpens is in the volume, then open door.
		OpenDoor();
	}

	// Check if its time to close the door.
	if (GetWorld()->GetTimeSeconds() >= LastDoorOpenTimeSec + DoorCloseDelaySec) {
		CloseDoor();
	}
	
}

