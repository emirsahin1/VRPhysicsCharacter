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
	sliderHand = nullptr; 
	sliderSpline = nullptr; 
	sliderGrabbed = false;
	canBolt = false;
	rightHandMain = false;
	interpSpeed = 400; 
	boltImpulse = 550;
	recoilIntensity = 700;
	impactIntensity = 1200;
	rollOffset = 0; 
	angularDriveParams = FVector(0.0f, 0.0f, 0.0f);

	gunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunBody"));
	mainGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("mainGrabCapsule"));
	altGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("altGrabCapsule"));
	sliderCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("sliderCapsule"));
	sliderSpline = CreateDefaultSubobject<USplineComponent>(TEXT("sliderSpline"));

	mainGrabPosition_R = CreateDefaultSubobject<USceneComponent>(TEXT("mainGrabPosition_R"));
	mainGrabPosition_L = CreateDefaultSubobject<USceneComponent>(TEXT("mainGrabPosition_L"));

	altGrabPosition_R = CreateDefaultSubobject<USceneComponent>(TEXT("altGrabPosition_R"));
	altGrabPosition_L = CreateDefaultSubobject<USceneComponent>(TEXT("altGrabPosition_L"));


	gunSlider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunSlider"));
	barrelPos = CreateDefaultSubobject<USceneComponent>(TEXT("barrelPos"));

	mainGrabCapsule->SetupAttachment(gunBody);
	altGrabCapsule->SetupAttachment(gunBody);
	sliderCapsule->SetupAttachment(gunBody);
	sliderSpline->SetupAttachment(gunBody);


	mainGrabPosition_R->SetupAttachment(gunBody);
	mainGrabPosition_L->SetupAttachment(gunBody);

	altGrabPosition_R->SetupAttachment(gunBody);
	altGrabPosition_L->SetupAttachment(gunBody);

	gunSlider->SetupAttachment(gunBody);
	barrelPos->SetupAttachment(gunBody);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	

}

void AWeapon::TriggerRight_Implementation() {
	if (rightHandMain) {
		FHitResult hitResult; 
		GetWorld()->LineTraceSingleByChannel(hitResult, barrelPos->GetComponentLocation(), barrelPos->GetForwardVector() * 10000.0f, ECollisionChannel::ECC_Visibility);
		DrawDebugLine(GetWorld(), barrelPos->GetComponentLocation(), barrelPos->GetForwardVector() * 10000.0f, FColor(255, 255, 0), false, 5.0f, 0U, 1.0f);
		gunBody->AddImpulse(gunBody->GetForwardVector() * recoilIntensity * -1.0f, FName(), false);
		
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		if (IsValid(hitComponent)) {
			hitComponent->AddImpulse((hitResult.TraceEnd - hitResult.TraceStart).GetSafeNormal() * impactIntensity, FName(), false);
		}
	}
}

void AWeapon::Grabbed_Implementation(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character) {
	
	if (grabCapsule->IsOverlappingComponent(mainGrabCapsule)) {

		FreeAngularMotors(handConstraint);

		if (IsValid(mainPhysicsConstraint)) { //If the main grip is already equiped, detach it. 
			Released(FName("None"), nullptr, nullptr, mainPhysicsConstraint, mainHandVisualConstraint, nullptr);
		}

		mainPhysicsConstraint = grabConstraint;
		mainHandVisualConstraint = handConstraint;

		if (hand == "Right") {
			rightHandMain = true;
			GrabAtLocation(mainGrabPosition_R, handMesh, mainPhysicsConstraint);
		}
		else {
			rightHandMain = false;
			GrabAtLocation(mainGrabPosition_L, handMesh, mainPhysicsConstraint);
		}
	}
	else if (grabCapsule->IsOverlappingComponent(altGrabCapsule)) {
		if (IsValid(mainPhysicsConstraint)) { //The main grip must be equipped for us to grip alt grip.
			FreeAngularMotors(handConstraint);
			secPhysicsConstraint = grabConstraint;
			secHandVisualConstraint = handConstraint;
			handConstraint->SetAngularDriveParams(angularDriveParams.X, angularDriveParams.Y, angularDriveParams.Z);

			if (hand == "Right") {
				GrabAtLocation(altGrabPosition_R, handMesh, secPhysicsConstraint);
			}
			else {
				GrabAtLocation(altGrabPosition_L, handMesh, secPhysicsConstraint);
			}
		}
	}
	else if (grabCapsule->IsOverlappingComponent(sliderCapsule)) {
		sliderGrabbed = true; 
		sliderHand = handMesh;
	}
	else { //Do a regular grab. 
		grabConstraint->SetConstrainedComponents(handMesh, FName(), gunBody, FName());
	}


}

void AWeapon::Released_Implementation(FName hand, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* grabCapsule, UPhysicsConstraintComponent* grabConstraint, UPhysicsConstraintComponent* handConstraint, AVRCharacter* character) {

	if (IsValid(handMesh) && handMesh == sliderHand) {
		sliderHand = nullptr;
		sliderGrabbed = false;
	}
	//Main hand is releasing, release both hands if alt grip is grabbed.
	else if (grabConstraint == mainPhysicsConstraint) {
		if (IsValid(secPhysicsConstraint)) {
			LockAngularMotors(secHandVisualConstraint);
			secPhysicsConstraint->BreakConstraint();
			secPhysicsConstraint = nullptr;
			secHandVisualConstraint->SetAngularDriveParams(character->orientStrength, character->velStrength, 0.0f);
			secHandVisualConstraint = nullptr;
		}
		LockAngularMotors(mainHandVisualConstraint);
		mainPhysicsConstraint->BreakConstraint();
		mainPhysicsConstraint = nullptr;
		mainHandVisualConstraint = nullptr;
	}
	else if (grabConstraint == secPhysicsConstraint) {
		LockAngularMotors(secHandVisualConstraint);
		secPhysicsConstraint->BreakConstraint();
		secPhysicsConstraint = nullptr;
		secHandVisualConstraint->SetAngularDriveParams(character->orientStrength, character->velStrength, 0.0f);
		secHandVisualConstraint = nullptr;
	}
	else {
		grabConstraint->BreakConstraint();
	}
}

void AWeapon::GrabAtLocation(USceneComponent* grabLocation, UInversePhysicsSkeletalMeshComponent* hand, UPhysicsConstraintComponent* grabConstraint) {
	hand->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FTransform grabLocTransform = grabLocation->GetComponentTransform();
	hand->SetWorldLocationAndRotation(grabLocTransform.GetLocation(), grabLocTransform.GetRotation(), false, (FHitResult*) nullptr, ETeleportType::TeleportPhysics);
	grabConstraint->SetConstrainedComponents(hand, FName(), gunBody, FName());
	
	hand->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AWeapon::FreeAngularMotors(UPhysicsConstraintComponent* handConstraint) {
	handConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	handConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
	handConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 0.0f);
}

void AWeapon::LockAngularMotors(UPhysicsConstraintComponent* handConstraint) {
	handConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	handConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
	handConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (sliderGrabbed) {
		FVector closestLoc = FMath::ClosestPointOnSegment(sliderHand->GetComponentLocation(), sliderSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World), sliderSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World));
		gunSlider->SetWorldLocation(closestLoc);
	}
	else {
		gunSlider->SetWorldLocation(FMath::VInterpConstantTo(gunSlider->GetComponentLocation(), sliderSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World), DeltaTime, interpSpeed));

		if (gunSlider->GetComponentLocation().Equals(sliderSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World), 0.01f)) {
			if (canBolt) {
				gunBody->AddImpulse(gunBody->GetForwardVector() * boltImpulse, FName(), false);
				canBolt = false;
			}
		}
		else {
			canBolt = true;
		}
	}
}
