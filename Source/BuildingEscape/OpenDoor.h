// Copyright Catlin Zilinski 2016

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;

	UPROPERTY(BlueprintAssignable)
	FOnCloseRequest OnCloseRequest;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// The angle to open the door to.
	float OpenAngle = -90.f;

private:
	const float GetTotalMassOfActorsOnPlate();

	// Rotates the door into the open angle and sets last opened time.
	void OpenDoor();

	// Rotates the door to the closed angle.
	void CloseDoor();

private:
	AActor * Owner = nullptr;

	UPROPERTY(EditAnywhere)
	// Reference to the trigger volume used to open the door.
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	// The amount of total mass required to trigger the pressure plate.
	float TriggerMass = 50.f;
};
