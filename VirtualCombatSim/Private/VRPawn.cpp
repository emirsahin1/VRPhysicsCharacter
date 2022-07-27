// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	rightControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("rightControllerMesh"));
	leftControllerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("leftControllerMesh"));


	VRCamera->SetupAttachment(VRTrackingCenter);
	leftController->SetupAttachment(VRTrackingCenter);
	rightController->SetupAttachment(VRTrackingCenter);
	leftConstraint->SetupAttachment(leftController);
	rightConstraint->SetupAttachment(rightController);
	rightControllerMesh->SetupAttachment(VRTrackingCenter);
	leftControllerMesh->SetupAttachment(VRTrackingCenter);
	rightPhysicsConstraint->SetupAttachment(rightConstraint);
	leftPhysicsConstraint->SetupAttachment(leftConstraint);

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	rightPhysicsConstraint->SetConstrainedComponents(rightConstraint, FName(), rightControllerMesh, FName());
	leftPhysicsConstraint->SetConstrainedComponents(leftConstraint, FName(), leftControllerMesh, FName());
	
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

