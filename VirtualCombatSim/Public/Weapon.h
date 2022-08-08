// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "VRInteractions.h"
#include "PhysSkeletalMeshActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class VIRTUALCOMBATSIM_API AWeapon : public AActor, public IVRInteractions
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void Grabbed(UPhysicsConstraintComponent* physicsConstraint, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* handCapsule);
	virtual void Grabbed_Implementation(UPhysicsConstraintComponent* physicsConstraint, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* handCapsule) override;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UCapsuleComponent* mainGrabCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UCapsuleComponent* altGrabCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UInversePhysicsSkeletalMeshComponent* mainGrabPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UInversePhysicsSkeletalMeshComponent* altGrabPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UStaticMeshComponent* gunBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UStaticMeshComponent* gunSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USceneComponent* barrelPos;
};
