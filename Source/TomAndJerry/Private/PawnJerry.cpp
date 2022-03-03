// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerry.h"
#include "WeaponMaterial.h"
#include "ControllerJerry.h"
#include "MinorObjective.h"

// Sets default values
APawnJerry::APawnJerry()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APawnJerry::BeginPlay()
{
	Super::BeginPlay();
	
	//Hide the weapon mesh that comes with the character mesh
	//GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	
}

// Called every frame
void APawnJerry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APawnJerry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnJerry::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APawnJerry::Strafe);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
}

void APawnJerry::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APawnJerry::Strafe(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

//Called when player walks over a weapon material or minor objective
//Calls the AddWeaponMaterial in ControllerJerry, and destroys the material
//Or activates the Minor Objective
void APawnJerry::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AWeaponMaterial* WeaponMaterial = Cast<AWeaponMaterial>(OtherActor);
	if (WeaponMaterial) {
		//UE_LOG(LogTemp, Warning, TEXT("Player walked over a Weapon Material!"));
		AControllerJerry* playerController = Cast<AControllerJerry>(GetController());
		if (playerController) {
			playerController->AddWeaponMaterial(WeaponMaterial->GetMaterialID());
			WeaponMaterial->Destroy();
		}
	}
	else {	//Not a Weapon Material. Check if we are standing on a Minor Objective
		AMinorObjective* MinorObjective = Cast<AMinorObjective>(OtherActor);
		if (MinorObjective) {
			MinorObjective->BeginActivation();
		}
	}
}

//Called when player walks off a Minor Objective
void APawnJerry::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	AMinorObjective* MinorObjective = Cast<AMinorObjective>(OtherActor);
	if (MinorObjective) {
		MinorObjective->StopActivation();
	}
}