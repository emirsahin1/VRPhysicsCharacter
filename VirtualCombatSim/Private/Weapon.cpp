// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mainPhysicsConstraint = nullptr;
	secPhysicsConstraint = nullptr;
	mainHandVisualConstraint = nullptr;
	secHandVisualConstraint = nullptr;
	rollOffset = 0; 
	angularDriveParams = FVector(0.0f, 0.0f, 0.0f);

	gunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunBody"));
	mainGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("mainGrabCapsule"));
	altGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("altGrabCapsule"));

	mainGrabPosition = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("mainGrabPosition"));
	altGrabPosition = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("altGrabPosition"));

	gunSlider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunSlider"));
	barrelPos = CreateDefaultSubobject<USceneComponent>(TEXT("barrelPos"));

	mainGrabCapsule->SetupAttachment(gunBody);
	altGrabCapsule->SetupAttachment(gunBody);

	mainGrabPosition->SetupAttachment(gunBody);
	altGrabPosition->SetupAttachment(gunBody);

	gunSlider->SetupAttachment(gunBody);
	barrelPos->SetupAttachment(gunBody);

	mainGrabPosition->SetVisibility(false);
	altGrabPosition->SetVisibility(false);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWeapon::Grabbed_Implementation(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character) {
	handConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	handConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	handConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);

	//For inverting the left hand. 
	if (hand == FName("Left")) {
		rollOffset = 180.0f;
	}
	else {
		rollOffset = 0.0f;
	}

	if (grabCapsule->IsOverlappingComponent(mainGrabCapsule)) {
		if (IsValid(mainPhysicsConstraint)) { //If the main grip is already equiped, detach it. 
			Released(FName("None"), nullptr, nullptr, mainPhysicsConstraint, handConstraint, nullptr);
		}
		mainPhysicsConstraint = grabConstraint;
		mainHandVisualConstraint = handConstraint;
		GrabAtLocation(mainGrabPosition, handMesh, mainPhysicsConstraint, true);
	}
	else if (grabCapsule->IsOverlappingComponent(altGrabCapsule)) {
		if (IsValid(mainPhysicsConstraint)) { //The main grip must be equipped for us to grip alt grip.
			secPhysicsConstraint = grabConstraint;
			secHandVisualConstraint = handConstraint;
			handConstraint->SetAngularDriveParams(angularDriveParams.X, angularDriveParams.Y, angularDriveParams.Z);
			GrabAtLocation(altGrabPosition, handMesh, secPhysicsConstraint, false);
		}
	}
	else { //Do a regular grab. 
		grabConstraint->SetConstrainedComponents(handMesh, FName(), gunBody, FName());
	}


}

void AWeapon::Released_Implementation(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character) {
	handConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	handConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	handConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);

	//Main hand is releasing, release both hands if alt grip is grabbed.
	if (grabConstraint == mainPhysicsConstraint) {
		if (IsValid(secPhysicsConstraint)) {
			secPhysicsConstraint->BreakConstraint();
			secPhysicsConstraint = nullptr;
			secHandVisualConstraint->SetAngularDriveParams(character->orientStrength, character->velStrength, 0.0f);
			secHandVisualConstraint = nullptr;
		}
		mainPhysicsConstraint->BreakConstraint();
		mainPhysicsConstraint = nullptr;
		mainHandVisualConstraint = nullptr;
	}
	else if (grabConstraint == secPhysicsConstraint) {
		secPhysicsConstraint->BreakConstraint();
		secPhysicsConstraint = nullptr;
		secHandVisualConstraint->SetAngularDriveParams(character->orientStrength, character->velStrength, 0.0f);
		secHandVisualConstraint = nullptr;
	}
	else {
		grabConstraint->BreakConstraint();
	}
}


// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::GrabAtLocation(UInversePhysicsSkeletalMeshComponent* grabLocation, UInversePhysicsSkeletalMeshComponent* hand, UPhysicsConstraintComponent* grabConstraint, bool isMain) {
	hand->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (isMain) {
		FTransform socketTransform = hand->GetSocketTransform(mainSocket, ERelativeTransformSpace::RTS_World);
		FRotator socketRotation = socketTransform.GetRotation().Rotator();
		socketRotation.Roll = socketRotation.Roll + rollOffset;
		//TODO:: Make this so that the gun attaches based on its own socket. 
		gunBody->SetWorldLocationAndRotation(socketTransform.GetLocation(), socketRotation, false, (FHitResult*) nullptr, ETeleportType::TeleportPhysics);
		grabConstraint->SetConstrainedComponents(hand, FName(), gunBody, FName());
	}
	else {
		FTransform grabLocTransform = grabLocation->GetComponentTransform();
		FRotator grabLocRotation = grabLocTransform.GetRotation().Rotator();
		grabLocRotation.Roll = grabLocRotation.Roll + rollOffset;
		hand->SetWorldLocationAndRotation(grabLocTransform.GetLocation(), grabLocRotation, false, (FHitResult*) nullptr, ETeleportType::TeleportPhysics);
		grabConstraint->SetConstrainedComponents(hand, FName(), gunBody, FName());
	}

	hand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}
