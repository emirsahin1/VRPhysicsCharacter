// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysSkeletalMeshActor.h"
#include "VRInteractions.h"
#include "VRCharacter.generated.h"

UCLASS()
class VIRTUALCOMBATSIM_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

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
		UInversePhysicsSkeletalMeshComponent* rightControllerPhysicsMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UInversePhysicsSkeletalMeshComponent* leftControllerPhysicsMesh;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* rightPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* leftPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* leftGrabPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* rightGrabPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCapsuleComponent* leftGrabCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCapsuleComponent* rightGrabCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float grabOffsetLimit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	AActor* leftGrabbedObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	AActor* rightGrabbedObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float orientStrength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float velStrength;

private:

	UFUNCTION()
		void MovementRightY(float value);

	UFUNCTION()
		void MovementRightX(float value);

	UFUNCTION()
		void SnapTurn(float value);

	UFUNCTION()
		void OnJump();

	UFUNCTION()
		void GrabLeft();

	UFUNCTION()
		void ReleaseLeft();

	UFUNCTION()
		void GrabRight();

	UFUNCTION()
		void ReleaseRight();

	void ReleaseObject(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, AActor* object, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint);

	AActor* GrabObject(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* gripCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, UCapsuleComponent* handCapsule);
	
	void OnMoveBegin();
	void OnMoveEnd();
	float GetHandToMeshDistance(UInversePhysicsSkeletalMeshComponent* handMesh, UMotionControllerComponent* controler);
	FVector hmdLoc;
	FVector capsuleLoc;
	bool physicsIsOff;
	bool canSnapTurn;


};
