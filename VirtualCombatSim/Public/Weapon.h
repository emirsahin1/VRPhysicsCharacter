// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "VRInteractions.h"
#include "PhysSkeletalMeshActor.h"
#include "VRCharacter.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SplineComponent.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimationAsset.h"
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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void Grabbed(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character);
	virtual void Grabbed_Implementation(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void Released(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character);
	virtual void Released_Implementation(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact|FromInteract")
	void TriggerRight();
	virtual void TriggerRight_Implementation();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UCapsuleComponent* mainGrabCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UCapsuleComponent* altGrabCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UCapsuleComponent* sliderCapsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USplineComponent* sliderSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USceneComponent* mainGrabPosition_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USceneComponent* mainGrabPosition_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USceneComponent* altGrabPosition_L;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USceneComponent* altGrabPosition_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UStaticMeshComponent* gunBody;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		UStaticMeshComponent* gunSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		USceneComponent* barrelPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		FName mainSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* mainAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		UAnimationAsset* secAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
		FVector angularDriveParams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float interpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float boltImpulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider")
		bool sliderGrabbed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float recoilIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Settings")
		float impactIntensity;

private:
	UPhysicsConstraintComponent* mainPhysicsConstraint; 
	UPhysicsConstraintComponent* secPhysicsConstraint;
	UPhysicsConstraintComponent* mainHandVisualConstraint;
	UPhysicsConstraintComponent* secHandVisualConstraint;
	UInversePhysicsSkeletalMeshComponent* sliderHand; 
	bool rightHandMain;
	float rollOffset;
	void GrabAtLocation(USceneComponent* grabLocation, UInversePhysicsSkeletalMeshComponent* hand, UPhysicsConstraintComponent* grabConstraint);
	void FreeAngularMotors(UPhysicsConstraintComponent* handConstraint);
	void LockAngularMotors(UPhysicsConstraintComponent* handConstraint);

	bool canBolt; 
};
