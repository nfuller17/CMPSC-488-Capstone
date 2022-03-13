// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerry.h"
#include "WeaponMaterial.h"
#include "ControllerJerry.h"
#include "MinorObjective.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"

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
	
	//Hide the weapon mesh that comes with the Wraith mesh
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Health = HealthMax;
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
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon1"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 1);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon2"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 2);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon3"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 3);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon4"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 4);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon5"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 5);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon6"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 6);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon7"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 7);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon8"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 8);
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon9"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 9);
	PlayerInputComponent->BindAction(TEXT("FirePrimary"), EInputEvent::IE_Pressed, this, &APawnJerry::BeginFire);
	PlayerInputComponent->BindAction(TEXT("FirePrimary"), EInputEvent::IE_Released, this, &APawnJerry::StopFire);
	PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeRight"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 4);
}

void APawnJerry::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APawnJerry::Strafe(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APawnJerry::Dodge(const int32 Direction)
{
	if (GetWorld()->GetTimeSeconds() - LastDodgeTime <= DodgeTime)
	{
		switch (Direction)
		{
			//W - forward
			case 1:
				if (!bSetForwardDodge)
				{
					bSetForwardDodge = true;
					bSetLeftDodge = false;
					bSetBackDodge = false;
					bSetRightDodge = false;
				}
				else	//Previously pressed W. Did player press it quick enough to dodge forward?
				{
					if (GetWorld()->GetTimeSeconds() - LastSuccessfulDodgeTime < TimeBetweenDodges)
						break;
					//GetActorForwardVector();
					UE_LOG(LogTemp, Warning, TEXT("Dodge F"));
					bSetForwardDodge = false;
					LastSuccessfulDodgeTime = GetWorld()->GetTimeSeconds();
				}
				break;
			//A - left
			case 2:
				if (!bSetLeftDodge)
				{
					bSetForwardDodge = false;
					bSetLeftDodge = true;
					bSetBackDodge = false;
					bSetRightDodge = false;
				}
				else
				{
					if (GetWorld()->GetTimeSeconds() - LastSuccessfulDodgeTime < TimeBetweenDodges)
						break;
					//-GetActorRightVector();
					UE_LOG(LogTemp, Warning, TEXT("Dodge L"));
					bSetLeftDodge = false;
					LastSuccessfulDodgeTime = GetWorld()->GetTimeSeconds();
				}
				break;
			//S - back
			case 3:
				if (!bSetBackDodge)
				{
					bSetForwardDodge = false;
					bSetLeftDodge = false;
					bSetBackDodge = true;
					bSetRightDodge = false;
				}
				else
				{
					if (GetWorld()->GetTimeSeconds() - LastSuccessfulDodgeTime < TimeBetweenDodges)
						break;
					//-GetActorForwardVector();
					UE_LOG(LogTemp, Warning, TEXT("Dodge B"));
					bSetBackDodge = false;
					LastSuccessfulDodgeTime = GetWorld()->GetTimeSeconds();
				}
				break;
			case 4:
				if (!bSetRightDodge)
				{
					bSetForwardDodge = false;
					bSetLeftDodge = false;
					bSetBackDodge = false;
					bSetRightDodge = true;
				}
				else
				{
					if (GetWorld()->GetTimeSeconds() - LastSuccessfulDodgeTime < TimeBetweenDodges)
						break;
					//GetActorRightVector();
					UE_LOG(LogTemp, Warning, TEXT("Dodge R"));
					bSetRightDodge = false;
					LastSuccessfulDodgeTime = GetWorld()->GetTimeSeconds();
				}
				break;
			default:
				bSetForwardDodge = false;
				bSetLeftDodge = false;
				bSetBackDodge = false;
				bSetRightDodge = false;
		}	
	}
	else
	{
		bSetForwardDodge = true;
		bSetLeftDodge = true;
		bSetBackDodge = true;
		bSetRightDodge = true;
	}
	LastDodgeTime = GetWorld()->GetTimeSeconds();
}

//Called when player walks over a weapon material or minor objective
//Calls the AddWeaponMaterial in ControllerJerry, and destroys the material
//Or activates the Minor Objective
void APawnJerry::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	AWeaponMaterial* WeaponMaterial = Cast<AWeaponMaterial>(OtherActor);
	if (WeaponMaterial)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player walked over a Weapon Material!"));
		AControllerJerry* playerController = Cast<AControllerJerry>(GetController());
		if (playerController) {
			playerController->AddWeaponMaterial(WeaponMaterial->GetMaterialID());
			WeaponMaterial->Destroy();
		}
	}
	else
	{	//Not a Weapon Material. Check if we are standing on a Minor Objective
		AMinorObjective* MinorObjective = Cast<AMinorObjective>(OtherActor);
		if (MinorObjective) {
			MinorObjective->SetActivatingPawn(this);
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

float APawnJerry::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToDo = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToDo = FMath::Min(Health, DamageToDo);
	Health -= DamageToDo;
	UE_LOG(LogTemp, Warning, TEXT("Player health: %f"), Health);
	//UNCOMMENT THIS AFTER DONE WITH TESTS!
	//if (Health <= 0)
	//{
	//	DetachFromControllerPendingDestroy();
	//	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//}
	return DamageToDo;
}

bool APawnJerry::IsDead() const
{
	return Health <= 0;
}

//Called by ObjectiveWeapon when a player has completed a Weapon Objective
//Adds the WeaponClass to the WeaponInventory array
void APawnJerry::AddWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	//Only add if player does not have this weapon
	for (auto wClass: WeaponInventory)
	{
		if (wClass == WeaponClass){
			UE_LOG(LogTemp, Warning, TEXT("Player already has this weapon - not adding."));
			return;
		}
	}
	WeaponInventory.Emplace(WeaponClass);
}

//Called when a player presses a number on the keyboard
//Checks if the player has the desired weapon
//If so, destroy the weapon being currently held and spawn an instance of the desired weapon
void APawnJerry::SelectWeapon(const int32 WeaponNumber)
{
	//Check if the currently held weapon is already selected
	if (Weapon)
	{
		if (Weapon->GetWeaponNumber() == WeaponNumber)
			return;
	}
	for (auto wClass: WeaponInventory)
	{
		if (wClass->GetDefaultObject<AWeapon>()->GetWeaponNumber() == WeaponNumber)	//wClass is a TSubclassOf template. Need to get the actual weapon class using GetDefaultObject
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon %d found!"), WeaponNumber);
			//Destroy the weapon the Weapon pointer is currently pointing to
			if (Weapon)
			{
				Weapon->StopFire();
				Weapon->Destroy();
			}
			//Spawn an instance of the selected Weapon
			Weapon = GetWorld()->SpawnActor<AWeapon>(wClass);
			Weapon->SetOwner(this);
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player does not have this weapon in inventory."));
}

//Called when a player clicks with left mouse
//Input binding is used here, as SetupPlayerInputComponent is called before BeginPlay and Weapon will be null
void APawnJerry::BeginFire()
{
	if (Weapon)
		Weapon->BeginFire();
}

//Called when a player releases left mouse
//Input binding is used here, as SetupPlayerInputComponent is called before BeginPlay and Weapon will be null
void APawnJerry::StopFire()
{
	if (Weapon)
		Weapon->StopFire();
}