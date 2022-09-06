// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	physicsIsOff = false;
	canSnapTurn = true;
	freeLocomotionLimit = 75.0f;
	hmdLoc = FVector(0.0f, 0.0f, 0.0f);
	capsuleLoc = FVector(0.0f, 0.0f, 0.0f);

	VRTrackingCenter = CreateDefaultSubobject<USceneComponent>(TEXT("VRTrackingCenter"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));

	VRCamera->SetupAttachment(VRTrackingCenter);
	VRTrackingCenter->SetupAttachment(ACharacter::GetCapsuleComponent());

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();

	FAttachmentTransformRules attachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, false);
	FActorSpawnParameters spawnParams = FActorSpawnParameters();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform spawnTrans = FTransform(VRTrackingCenter->GetComponentRotation(), VRTrackingCenter->GetComponentLocation(), FVector::OneVector);

	rightPhysicsHand = GetWorld()->SpawnActorDeferred<APhysicsHand>(handClass.Get(), spawnTrans, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	leftPhysicsHand = GetWorld()->SpawnActorDeferred<APhysicsHand>(handClass.Get(), spawnTrans, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	rightPhysicsHand->handSource = FName("Right");
	leftPhysicsHand->handSource = FName("Left");
	rightPhysicsHand->SetupHand();
	leftPhysicsHand->SetupHand();
	rightPhysicsHand->FinishSpawning(spawnTrans);
	leftPhysicsHand->FinishSpawning(spawnTrans);

	rightPhysicsHand->AttachToComponent(VRTrackingCenter, attachmentRules);
	leftPhysicsHand->AttachToComponent(VRTrackingCenter, attachmentRules);
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

	if (offset.Size() > freeLocomotionLimit) {
		AddActorWorldOffset(FVector(offsetX, offsetY, 0.0f), true, nullptr, ETeleportType::None);
		VRTrackingCenter->AddWorldOffset(FVector(offsetX * -1.0f, offsetY * -1.0f, 0.0f), false, nullptr, ETeleportType::None);
	}

	//ACharacter::GetCapsuleComponent()->SetCapsuleHalfHeight(hmdLoc.Z / 2.0f, true);
	
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


void AVRCharacter::GrabLeft() {
	leftPhysicsHand->GrabObject();
}

void AVRCharacter::GrabRight() {
	rightPhysicsHand->GrabObject();
}

void AVRCharacter::ReleaseLeft() {
	leftPhysicsHand->ReleaseObject();
}

void AVRCharacter::ReleaseRight() {
	rightPhysicsHand->ReleaseObject();
}


void AVRCharacter::OnJump() {
	//OnMoveBegin();
	Jump();
}

void AVRCharacter::OnMoveBegin() {
	rightPhysicsHand->OnMoveBegin();
	leftPhysicsHand->OnMoveBegin();
}

void AVRCharacter::OnMoveEnd() {
	rightPhysicsHand->OnMoveEnd();
	leftPhysicsHand->OnMoveEnd();
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
			FAttachmentTransformRules transformRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
		
			//There is probably a cleaner way of writing this code :)
			if (IsValid(leftPhysicsHand->grabbedObject)) {
				Cast<UPrimitiveComponent>(leftPhysicsHand->grabbedObject->GetRootComponent())->SetSimulatePhysics(false);
				leftPhysicsHand->grabbedObject->AttachToComponent(leftPhysicsHand->motionController, transformRules);
			}
			if (IsValid(rightPhysicsHand->grabbedObject)) {
				Cast<UPrimitiveComponent>(rightPhysicsHand->grabbedObject->GetRootComponent())->SetSimulatePhysics(false);
				rightPhysicsHand->grabbedObject->AttachToComponent(rightPhysicsHand->motionController, transformRules);
			}
			GetCapsuleComponent()->AddWorldRotation(FRotator(0.0f, value * 45.0f, 0.0f), false, nullptr, ETeleportType::TeleportPhysics);
			canSnapTurn = false;

			if (IsValid(leftPhysicsHand->grabbedObject)) {
				//leftGrabbedObject->DetachRootComponentFromParent();
				leftPhysicsHand->grabbedObject->DetachFromActor(FDetachmentTransformRules(transformRules, true));
				Cast<UPrimitiveComponent>(leftPhysicsHand->grabbedObject->GetRootComponent())->SetSimulatePhysics(true);

			}
			if (IsValid(rightPhysicsHand->grabbedObject)) {
				//rightGrabbedObject->DetachRootComponentFromParent();
				rightPhysicsHand->grabbedObject->DetachFromActor(FDetachmentTransformRules(transformRules, true));
				Cast<UPrimitiveComponent>(rightPhysicsHand->grabbedObject->GetRootComponent())->SetSimulatePhysics(true);
			}
			OnMoveEnd();
		}
	}
	else {
		canSnapTurn = true;
	}
}
