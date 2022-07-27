// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GripMotionControllerComponent.h"
#include "Engine/Engine.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/ActorChannel.h"
#include "PhysSkeletalMeshActor.generated.h"


/**
* A component specifically for being able to turn off movement replication in the component at will
* Has the upside of also being a blueprintable base since UE4 doesn't allow that with std ones
*/

USTRUCT()
struct FSkeletalMeshComponentEndPhysicsTickFunctionVR : public FTickFunction
{
	GENERATED_USTRUCT_BODY()

		UInversePhysicsSkeletalMeshComponent* TargetVR;

	virtual void ExecuteTick(float DeltaTime, enum ELevelTick TickType, ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) override;

	virtual FString DiagnosticMessage() override;

	virtual FName DiagnosticContext(bool bDetailed) override;

};
template<>
struct TStructOpsTypeTraits<FSkeletalMeshComponentEndPhysicsTickFunctionVR> : public TStructOpsTypeTraitsBase2<FSkeletalMeshComponentEndPhysicsTickFunctionVR>
{
	enum
	{
		WithCopy = false
	};
};

// A base skeletal mesh component that has been added to temp correct an engine bug with inversed scale and physics
UCLASS(Blueprintable, meta = (ChildCanTick, BlueprintSpawnableComponent), ClassGroup = (VRExpansionPlugin))
class VIRTUALCOMBATSIM_API UInversePhysicsSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	UInversePhysicsSkeletalMeshComponent(const FObjectInitializer& ObjectInitializer);

public:

	// Overrides the default of : true and allows for controlling it like in an actor, should be default of off normally with grippable components
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Component Replication")
		bool bReplicateMovement;

	// This is all overrides to fix the skeletal mesh inverse simulation bug
	// WILL BE REMOVED LATER when the engine is fixed
	FSkeletalMeshComponentEndPhysicsTickFunctionVR EndPhysicsTickFunctionVR;
	friend struct FSkeletalMeshComponentEndPhysicsTickFunctionVR;
	void EndPhysicsTickComponentVR(FSkeletalMeshComponentEndPhysicsTickFunctionVR& ThisTickFunction);
	void BlendInPhysicsInternalVR(FTickFunction& ThisTickFunction);
	void FinalizeAnimationUpdateVR();

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override
	{
		// Get rid of inverse issues
		FTransform newLocalToWorld = LocalToWorld;
		newLocalToWorld.SetScale3D(newLocalToWorld.GetScale3D().GetAbs());

		return Super::CalcBounds(newLocalToWorld);
	}

	UFUNCTION(BlueprintPure, Category = "VRExpansionFunctions")
		FBoxSphereBounds GetLocalBounds() const
	{
		return this->GetCachedLocalBounds();
	}

	void PerformBlendPhysicsBonesVR(const TArray<FBoneIndexType>& InRequiredBones, TArray<FTransform>& InBoneSpaceTransforms);
	virtual void RegisterEndPhysicsTick(bool bRegister) override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// END INVERSED MESH FIX


};

