// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mainGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("mainGrabCapsule"));
	altGrabCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("altGrabCapsule"));

	mainGrabPosition = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("mainGrabPosition"));
	altGrabPosition = CreateDefaultSubobject<UInversePhysicsSkeletalMeshComponent>(TEXT("altGrabPosition"));

	gunBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunBody"));
	gunSlider = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunSlider"));
	barrelPos = CreateDefaultSubobject<USceneComponent>(TEXT("barrelPos"));

	mainGrabCapsule->SetupAttachment(gunBody);
	altGrabCapsule->SetupAttachment(gunBody);

	mainGrabPosition->SetupAttachment(gunBody);
	altGrabPosition->SetupAttachment(gunBody);

	gunSlider->SetupAttachment(gunBody);
	barrelPos->SetupAttachment(gunBody);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

//
//void AWeapon::Grabbed_Implementation(UPhysicsConstraintComponent* physicsConstraint, UInversePhysicsSkeletalMeshComponent* handMesh, UCapsuleComponent* handCapsule) {
//
//}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

