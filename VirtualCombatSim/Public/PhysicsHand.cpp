// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsHand.h"

// Sets default values
APhysicsHand::APhysicsHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	physicsIsOff = false;
	isGrabbing = false;
	grabOffsetLimit = 30.0f;
	orientStrength = 10000.0;
	velStrength = 1000.0;
	handSource = FName("Right");

	motionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("motionController"));
	motionController->SetTrackingMotionSource(FName("Right")); //TODO::Make this dynamic

	constraintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("constraintMesh"));
	handMesh = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("handMesh"));
	handPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("handPhysicsConstraint"));
	grabPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("rightGrabPhysicsConstraint"));
	grabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("grabCapsule"));

	handMesh->SetSimulatePhysics(true);

	constraintMesh->SetupAttachment(motionController);
	handMesh->SetupAttachment(motionController);
	grabCapsule->SetupAttachment(handMesh);
	grabPhysicsConstraint->SetupAttachment(motionController);
	handPhysicsConstraint->SetupAttachment(constraintMesh);

	grabPhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	grabPhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	grabPhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);


	handPhysicsConstraint->SetDisableCollision(true);
	handPhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);
	handPhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);
	handPhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);

	handPhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	handPhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	handPhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	handPhysicsConstraint->SetLinearPositionTarget(FVector(0.0f, 0.0f, 0.0f));
	handPhysicsConstraint->SetLinearPositionDrive(true, true, true);
	handPhysicsConstraint->SetLinearDriveParams(2000.0f, 200.0f, 0.0f);

	handPhysicsConstraint->SetAngularDriveParams(500.0f, 50.0f, 0.0f);
	handPhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	handPhysicsConstraint->SetAngularOrientationDrive(true, true);
	handPhysicsConstraint->SetAngularVelocityDrive(true, true);

	
}

// Called when the game starts or when spawned
void APhysicsHand::BeginPlay()
{
	Super::BeginPlay();
	handPhysicsConstraint->SetConstrainedComponents(constraintMesh, FName(), handMesh, FName());
}

void APhysicsHand::SetupHand() {
	motionController->SetTrackingMotionSource(handSource);
	if (handSource == "Left") {
		handMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
	}
}

// Called every frame
void APhysicsHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (motionController->CurrentTrackingStatus == ETrackingStatus::NotTracked) {
		handMesh->SetSimulatePhysics(false);

	}
	else if (!handMesh->IsSimulatingPhysics()) {
		handMesh->SetSimulatePhysics(true);
	}

	if (grabbedObject != nullptr && GetControllerToHandDistance(handMesh, motionController) > grabOffsetLimit) {
		ReleaseObject();
	}
}

AActor* APhysicsHand::GrabObject() {
	isGrabbing = true;
	AActor* closestObject = nullptr;
	float minDistance = 0.0f;
	TSet<AActor*> overlaps = TSet<AActor*>();

	grabCapsule->GetOverlappingActors(overlaps);

	if (overlaps.Num() != 0) {

		for (AActor* actor : overlaps) {
			if (actor->Implements<UVRInteractions>()) {

				if (closestObject == nullptr) {
					closestObject = actor;
					minDistance = (actor->GetActorLocation() - handMesh->GetComponentLocation()).Size();
				}
				else {
					float distance = (actor->GetActorLocation() - handMesh->GetComponentLocation()).Size();
					if (distance < minDistance) {
						minDistance = distance;
						closestObject = actor;
					}
				}
			}
		}
		if (IsValid(closestObject)) {
			grabbedObject = closestObject;
			IVRInteractions::Execute_Grabbed(closestObject, this);
			return closestObject;
		}
	}
	return nullptr;
}

void APhysicsHand::ReleaseObject() {
	isGrabbing = false;
	if (IsValid(grabbedObject)) {
		IVRInteractions::Execute_Released(grabbedObject, this);
		grabbedObject = nullptr;
	}
}

void APhysicsHand::OnMoveBegin() {
	if (!physicsIsOff) {
		FAttachmentTransformRules transformRulesWorldAttach = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		handMesh->SetSimulatePhysics(false);
		handMesh->AttachToComponent(motionController, transformRulesWorldAttach);
		physicsIsOff = true;
	}
}

void APhysicsHand::OnMoveEnd() {
	if (physicsIsOff) {
		physicsIsOff = false;
		handMesh->SetSimulatePhysics(true);
	}
}



float APhysicsHand::GetControllerToHandDistance(UInversePhysicsSkeletalMeshComponent* mesh, UMotionControllerComponent* controller) {
	return (mesh->GetComponentLocation() - controller->GetComponentLocation()).Size();
}

