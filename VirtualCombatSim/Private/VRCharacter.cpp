// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	physicsIsOff = false;
	canSnapTurn = true;
	grabOffsetLimit = 30.0f;
	orientStrength = 10000.0;
	velStrength = 1000.0; 
	hmdLoc = FVector(0.0f, 0.0f, 0.0f);
	capsuleLoc = FVector(0.0f, 0.0f, 0.0f);

	VRTrackingCenter = CreateDefaultSubobject<USceneComponent>(TEXT("VRTrackingCenter"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));

	leftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("leftController"));
	leftController->SetTrackingMotionSource(FName("Left"));
	rightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("rightController"));
	rightController->SetTrackingMotionSource(FName("Right"));

	leftConstraint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftConstraint"));
	rightConstraint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightConstraint"));

	rightPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("rightPhysicsConstraint"));
	leftPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("leftPhysicsConstraint"));

	rightGrabPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("rightGrabPhysicsConstraint"));
	leftGrabPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("leftGrabPhysicsConstraint"));


	rightControllerPhysicsMesh = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("rightControllerPhysicsMesh"));
	leftControllerPhysicsMesh = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("leftControllerPhysicsMesh"));

	leftGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("leftGrabCapsule"));
	rightGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("rightGrabCapsule"));


	rightControllerPhysicsMesh->SetSimulatePhysics(true);
	leftControllerPhysicsMesh->SetSimulatePhysics(true);

	VRCamera->SetupAttachment(VRTrackingCenter);
	leftController->SetupAttachment(VRTrackingCenter);
	rightController->SetupAttachment(VRTrackingCenter);

	rightConstraint->SetupAttachment(rightController);
	leftConstraint->SetupAttachment(leftController);

	leftGrabPhysicsConstraint->SetupAttachment(leftController);
	rightGrabPhysicsConstraint->SetupAttachment(rightController);

	rightControllerPhysicsMesh->SetupAttachment(rightController);
	rightGrabCapsule->SetupAttachment(rightControllerPhysicsMesh);

	leftControllerPhysicsMesh->SetupAttachment(leftController);
	leftGrabCapsule->SetupAttachment(leftControllerPhysicsMesh);

	rightPhysicsConstraint->SetupAttachment(rightConstraint);
	leftPhysicsConstraint->SetupAttachment(leftConstraint);
	VRTrackingCenter->SetupAttachment(ACharacter::GetCapsuleComponent());


	rightGrabPhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	rightGrabPhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	rightGrabPhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	leftGrabPhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	leftGrabPhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	leftGrabPhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);


	rightPhysicsConstraint->SetDisableCollision(true);
	rightPhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);
	rightPhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);
	rightPhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);

	rightPhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	rightPhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	rightPhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	rightPhysicsConstraint->SetLinearPositionTarget(FVector(0.0f, 0.0f, 0.0f));
	rightPhysicsConstraint->SetLinearPositionDrive(true, true, true);
	rightPhysicsConstraint->SetLinearDriveParams(2000.0f, 200.0f, 0.0f);

	rightPhysicsConstraint->SetAngularDriveParams(500.0f, 50.0f, 0.0f);
	rightPhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	rightPhysicsConstraint->SetAngularOrientationDrive(true, true);
	rightPhysicsConstraint->SetAngularVelocityDrive(true, true);

	leftPhysicsConstraint->SetDisableCollision(true);
	leftPhysicsConstraint->SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);
	leftPhysicsConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);
	leftPhysicsConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 90.0f);

	leftPhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	leftPhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 45.0f);
	leftPhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 45.0f);

	leftPhysicsConstraint->SetLinearPositionTarget(FVector(0.0f, 0.0f, 0.0f));
	leftPhysicsConstraint->SetLinearPositionDrive(true, true, true);
	leftPhysicsConstraint->SetLinearDriveParams(2000.0f, 200.0f, 0.0f);

	leftPhysicsConstraint->SetAngularDriveParams(500.0f, 50.0f, 0.0f);
	leftPhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	leftPhysicsConstraint->SetAngularOrientationDrive(true, true);
	leftPhysicsConstraint->SetAngularVelocityDrive(true, true);

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	rightPhysicsConstraint->SetConstrainedComponents(rightConstraint, FName(), rightControllerPhysicsMesh, FName());
	leftPhysicsConstraint->SetConstrainedComponents(leftConstraint, FName(), leftControllerPhysicsMesh, FName());
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::FromInt(GetCharacterMovement()->Velocity.Size()));
	
	hmdLoc = VRCamera->GetComponentLocation();
	capsuleLoc = ACharacter::GetCapsuleComponent()->GetComponentLocation();
	float offsetX = hmdLoc.X - capsuleLoc.X;
	float offsetY = hmdLoc.Y - capsuleLoc.Y;
	FVector2D offset = FVector2D(offsetX, offsetY);

	if (offset.Size() > 15.0f) {
		AddActorWorldOffset(FVector(offsetX, offsetY, 0.0f), true, nullptr, ETeleportType::None);
		VRTrackingCenter->AddWorldOffset(FVector(offsetX * -1.0f, offsetY * -1.0f, 0.0f), false, nullptr, ETeleportType::None);
	}


	//ACharacter::GetCapsuleComponent()->SetCapsuleHalfHeight(hmdLoc.Z / 2.0f, true);
	

	if (leftController->CurrentTrackingStatus == ETrackingStatus::NotTracked) {
		leftControllerPhysicsMesh->SetSimulatePhysics(false);
	
	}
	else if(!leftControllerPhysicsMesh->IsSimulatingPhysics()){
		leftControllerPhysicsMesh->SetSimulatePhysics(true);
	}

	if (rightController->CurrentTrackingStatus == ETrackingStatus::NotTracked) {
		rightControllerPhysicsMesh->SetSimulatePhysics(false);
	}
	else if (!rightControllerPhysicsMesh->IsSimulatingPhysics()) {
		rightControllerPhysicsMesh->SetSimulatePhysics(true);
	}

	if (leftGrabbedObject != nullptr && GetHandToMeshDistance(leftControllerPhysicsMesh, leftController) > grabOffsetLimit) {
		ReleaseLeft();
	}

	if (rightGrabbedObject != nullptr && GetHandToMeshDistance(rightControllerPhysicsMesh, rightController) > grabOffsetLimit) {
		ReleaseRight();
	}

}

// Called to bind functionality to input
void AVRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MovementAxisRight_Y"), this, &AVRCharacter::MovementRightY);
	PlayerInputComponent->BindAxis(TEXT("MovementAxisRight_X"), this, &AVRCharacter::MovementRightX);
	PlayerInputComponent->BindAxis(TEXT("MovementAxisLeft_X"), this, &AVRCharacter::SnapTurn);
	PlayerInputComponent->BindAction(TEXT("ARight"), EInputEvent::IE_Pressed, this, &AVRCharacter::OnJump);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), EInputEvent::IE_Pressed, this, &AVRCharacter::GrabLeft);
	PlayerInputComponent->BindAction(TEXT("GrabLeft"), EInputEvent::IE_Released, this, &AVRCharacter::ReleaseLeft);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), EInputEvent::IE_Pressed, this, &AVRCharacter::GrabRight);
	PlayerInputComponent->BindAction(TEXT("GrabRight"), EInputEvent::IE_Released, this, &AVRCharacter::ReleaseRight);
}

float AVRCharacter::GetHandToMeshDistance(UInversePhysicsSkeletalMeshComponent* handMesh, UMotionControllerComponent* controller) {
	return (handMesh->GetComponentLocation() - controller->GetComponentLocation()).Size();
}

void AVRCharacter::GrabLeft() {
	leftGrabbedObject = GrabObject(FName("Left"), leftControllerPhysicsMesh, leftGrabCapsule, leftGrabPhysicsConstraint, leftPhysicsConstraint, leftGrabCapsule);
}

void AVRCharacter::GrabRight() {
	rightGrabbedObject = GrabObject(FName("Right"), rightControllerPhysicsMesh, rightGrabCapsule, rightGrabPhysicsConstraint, rightPhysicsConstraint, rightGrabCapsule);
}

void AVRCharacter::ReleaseLeft() {
	ReleaseObject(FName("Left"), leftControllerPhysicsMesh, leftGrabCapsule, leftGrabbedObject, leftGrabPhysicsConstraint, leftPhysicsConstraint);
	leftGrabbedObject = nullptr;
}

void AVRCharacter::ReleaseRight() {
	ReleaseObject(FName("Right"), rightControllerPhysicsMesh, rightGrabCapsule, rightGrabbedObject, rightGrabPhysicsConstraint, rightPhysicsConstraint);
	rightGrabbedObject = nullptr;
}

AActor* AVRCharacter::GrabObject(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh,  UCapsuleComponent* gripCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, UCapsuleComponent* handCapsule) {
	
	AActor* closestObject = nullptr;
	float minDistance = 0.0f;
	TSet<AActor*> overlaps = TSet<AActor*>();

	gripCapsule->GetOverlappingActors(overlaps);

	if (overlaps.Num() != 0) {

		for (AActor* actor : overlaps) {
			if(actor->Implements<UVRInteractions>()){

				if (closestObject == nullptr) {
					closestObject = actor;
					minDistance = (actor->GetActorLocation() - leftControllerPhysicsMesh->GetComponentLocation()).Size();
				}
				else {
					float distance = (actor->GetActorLocation() - leftControllerPhysicsMesh->GetComponentLocation()).Size();
					if (distance < minDistance) {
						minDistance = distance;
						closestObject = actor; 
					}
				}
			}
		}
		if (IsValid(closestObject)) {
			//Cast<IVRInteractions>(closestObject)->Grabbed(constraint, handMesh);
			/*FAttachmentTransformRules transformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
			if (leftGrabbedObject != closestObject && rightGrabbedObject != closestObject) {
				closestObject->AttachToComponent(handMesh, transformRules);
			}*/
			//closestObject->GetComponentsByTag(TSubclassOf<UShapeComponent>(), FName("MainGrip"));
			IVRInteractions::Execute_Grabbed(closestObject, hand, handMesh, gripCapsule, grabConstraint, handConstraint, this);
			return closestObject;
		}
	}
	return nullptr;
}

void AVRCharacter::ReleaseObject(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, AActor* object, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint) {
	if (IsValid(object)) {
		IVRInteractions::Execute_Released(object, hand, handMesh, grabCapsule, grabConstraint, handConstraint, this);
	}
}

void AVRCharacter::OnJump() {
	//OnMoveBegin();
	Jump();
}

void AVRCharacter::OnMoveBegin() {

	if (!physicsIsOff) {
		rightControllerPhysicsMesh->SetSimulatePhysics(false);
		leftControllerPhysicsMesh->SetSimulatePhysics(false);

		FAttachmentTransformRules transformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		rightControllerPhysicsMesh->AttachToComponent(rightController, transformRules);
		leftControllerPhysicsMesh->AttachToComponent(leftController, transformRules);
		physicsIsOff = true;
	}
}

void AVRCharacter::OnMoveEnd() {
	if (physicsIsOff) {
		physicsIsOff = false;
		rightControllerPhysicsMesh->SetSimulatePhysics(true);
		leftControllerPhysicsMesh->SetSimulatePhysics(true);
	}
}

void AVRCharacter::MovementRightY(float value) {
	//OnMoveBegin();
	FVector fwdVector = VRCamera->GetForwardVector();
	AddMovementInput(FVector(fwdVector.X * 10.0f, fwdVector.Y * 10.0f, 0.0f), value / 20.0f);
}

void AVRCharacter::MovementRightX(float value) {
	//OnMoveBegin();
	FVector rightVector = VRCamera->GetRightVector();
	AddMovementInput(FVector(rightVector.X * 10.0f, rightVector.Y * 10.0f, 0.0f), value / 20.0f);
}

void AVRCharacter::SnapTurn(float value) {

	if (fabsf(value) >= 0.8f){
		if (canSnapTurn) {
			OnMoveBegin();
			FRotator(0.0f, 0.0f, value);
		
			//There is probably a cleaner way of writing this code :)
			if (leftGrabbedObject != nullptr) {
				Cast<UPrimitiveComponent>(leftGrabbedObject->GetRootComponent())->SetSimulatePhysics(false);
				FAttachmentTransformRules transformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
				leftGrabbedObject->AttachToComponent(leftController, transformRules);
			}
			if (rightGrabbedObject != nullptr) {
				Cast<UPrimitiveComponent>(rightGrabbedObject->GetRootComponent())->SetSimulatePhysics(false);
				FAttachmentTransformRules transformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
				rightGrabbedObject->AttachToComponent(rightController, transformRules);
			}
			GetCapsuleComponent()->AddWorldRotation(FRotator(0.0f, value * 45.0f, 0.0f), false, nullptr, ETeleportType::TeleportPhysics);
			canSnapTurn = false;

			if (leftGrabbedObject != nullptr) {
				leftGrabbedObject->DetachRootComponentFromParent();
				Cast<UPrimitiveComponent>(leftGrabbedObject->GetRootComponent())->SetSimulatePhysics(true);

			}
			if (rightGrabbedObject != nullptr) {
				rightGrabbedObject->DetachRootComponentFromParent();
				Cast<UPrimitiveComponent>(rightGrabbedObject->GetRootComponent())->SetSimulatePhysics(true);
			}
			OnMoveEnd();

		}
	}
	else {
		canSnapTurn = true;
	}
}



//TODO:: Fix snap physics. 