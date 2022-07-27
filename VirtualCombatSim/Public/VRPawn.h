// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VRPawn.generated.h"

UCLASS()
class VIRTUALCOMBATSIM_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USceneComponent* VRTrackingCenter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCameraComponent* VRCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UMotionControllerComponent* leftController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UMotionControllerComponent* rightController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* rightConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* leftConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* rightControllerMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* leftControllerMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* rightPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* leftPhysicsConstraint;

};
