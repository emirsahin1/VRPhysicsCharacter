// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysSkeletalMeshActor.h"
#include "VRInteractions.h"
#include "GameFramework/Actor.h"
#include "PhysicsHand.generated.h"

UCLASS()
class VIRTUALCOMBATSIM_API APhysicsHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsHand();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UMotionControllerComponent* motionController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* constraintMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UInversePhysicsSkeletalMeshComponent* handMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* handPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UPhysicsConstraintComponent* grabPhysicsConstraint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCapsuleComponent* grabCapsule;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float grabOffsetLimit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName handSource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		AActor* grabbedObject;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float orientStrength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float velStrength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		bool isGrabbing;

	UFUNCTION(BlueprintCallable)
		AActor* GrabObject();

	UFUNCTION(BlueprintCallable)
		void ReleaseObject();

	UFUNCTION(BlueprintCallable)
		void OnMoveBegin();

	UFUNCTION(BlueprintCallable)
		void OnMoveEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float GetControllerToHandDistance(UInversePhysicsSkeletalMeshComponent* mesh, UMotionControllerComponent* controller);

	bool physicsIsOff;
};
