// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "PhysSkeletalMeshActor.h"
#include "Components/CapsuleComponent.h"
//#include "VRCharacter.h"
#include "UObject/Interface.h"
#include "VRInteractions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVRInteractions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VIRTUALCOMBATSIM_API IVRInteractions
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void Grabbed(APhysicsHand* hand);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void Released(APhysicsHand* hand);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void TriggerRight();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void ButtonOneRight();

};
