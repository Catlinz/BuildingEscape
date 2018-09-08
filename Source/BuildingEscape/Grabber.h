// Copyright Catlin Zilinski 2016

#pragma once

#include "CoreMinimal.h"

#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"

#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	// Ray-cast and grab whats in reach.
	void Grab();

	// Called when grab is released.
	void Release();

	// Find the attached physics handle component.
	void FindPhysicsHandleComponent();
	
	// Find the attached input component and setup input handling.
	void SetupInputComponent();

	// Return hit for first physics body in reach.
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current end of reach line
	const FVector GetReachLineEnd();

	// Returns current start of reach line
	const FVector GetReachLineStart();

private:
	// The distance (in cm) we can grab an object from.
	UPROPERTY(EditAnywhere)
	int GrabReachCm = 100;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
};
