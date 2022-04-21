// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerry.h"
#include "WeaponMaterial.h"
#include "ControllerJerry.h"
#include "MinorObjective.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"
#include "PawnMonster.h"
#include "GameFramework/GameModeBase.h"
#include "../TomAndJerryGameModeBase.h"
#include "Skill_Amplify.h"

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
	AmmoComponent = FindComponentByClass<UAmmoComponent>();
	Health = HealthMax;

	//Start energy regen timer
	GetWorldTimerManager().SetTimer(EnergyTimer, this, &APawnJerry::AddEnergyHelper, EnergyRegenRate, true, 0.0);

	//If we respawned after dying and we had a super weapon, re-create our super weapon
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (Game != nullptr && Game->HasSuperWeapon())
	{
		Game->SpawnSuperWeapon(this);
	}
}

// Called every frame
void APawnJerry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnJerry::DestroyOnSpectate(const bool& SpectateMode)
{
	if (SpectateMode)
		Destroy();
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
	PlayerInputComponent->BindAction<FSelectWeaponDelegate>(TEXT("SelectWeapon0"), EInputEvent::IE_Pressed, this, &APawnJerry::SelectWeapon, 0);
	PlayerInputComponent->BindAction(TEXT("FirePrimary"), EInputEvent::IE_Pressed, this, &APawnJerry::BeginFire);
	PlayerInputComponent->BindAction(TEXT("FirePrimary"), EInputEvent::IE_Released, this, &APawnJerry::StopFire);
	PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeForward"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 1);
	PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeLeft"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 2);
	PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeBackward"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 3);
	PlayerInputComponent->BindAction<FDodgeDelegate>(TEXT("DodgeRight"), EInputEvent::IE_Pressed, this, &APawnJerry::Dodge, 4);
	PlayerInputComponent->BindAction(TEXT("OpenMenu"), EInputEvent::IE_Pressed, this, &APawnJerry::OpenMenu);
	PlayerInputComponent->BindAction(TEXT("NextSkill"), EInputEvent::IE_Pressed, this, &APawnJerry::NextSkill);
	PlayerInputComponent->BindAction(TEXT("PreviousSkill"), EInputEvent::IE_Pressed, this, &APawnJerry::PreviousSkill);
	PlayerInputComponent->BindAction(TEXT("DoSkill"), EInputEvent::IE_Pressed, this, &APawnJerry::ExecuteSkill);
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
	if (IsDead())
		return;
	if (!CanJump())
		return;
	if (GetWorld()->GetTimeSeconds() - LastDodgeTime >= TimeBetweenDodges) {	// Checks to see if enough time has passed to perform a dodge
		if (GetWorld()->GetTimeSeconds() - LastDirInputTime <= DoublePressTime) {	// Checks to see if two direction inputs occur in rapid succession
			if (Direction == prevDir) {		// If the previous input direction is the same as the current one...
				LastDodgeTime = GetWorld()->GetTimeSeconds();
				FVector DodgeDirection;
				switch (Direction) {
				case 1:	// W - forward
					DodgeDirection = (GetActorForwardVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				case 2: // A - left
					DodgeDirection = (-GetActorRightVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				case 3: // S - backwards
					DodgeDirection = (-GetActorForwardVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				case 4:	// D - right
					DodgeDirection = (GetActorRightVector() * DodgeStrength) + FVector(0, 0, ZAdd);
					break;
				}
				LaunchCharacter(DodgeDirection, false, false);
			}
		}
	}
	prevDir = Direction;	// Stores the current inputted direction
	LastDirInputTime = GetWorld()->GetTimeSeconds();
}

//Called when player walks over a weapon material or minor objective
//Calls the AddWeaponMaterial in ControllerJerry, and destroys the material
//Or activates the Minor Objective
void APawnJerry::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (IsDead())
		return;
	AWeaponMaterial* WeaponMaterial = Cast<AWeaponMaterial>(OtherActor);
	if (WeaponMaterial)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player walked over a Weapon Material!"));
		AddWeaponMaterial(WeaponMaterial->GetFactory());
		WeaponMaterial->Destroy();
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
	if (IsDead() || bInvulnerable)
		return 0.0;
	//Check friendly fire
	//Checking EventInstigator not null is particularly important since AI controllers can be destroyed before we take damage, e.g. projectiles traveling
	if (EventInstigator != nullptr)
	{
		APawnMonster* Monster = Cast<APawnMonster>(EventInstigator->GetPawn());
		if (Monster != nullptr && Monster->IsPlayerTeam())	//Friendly AI
			return 0.0;
	}

	DamageToDo = FMath::Min(Health, DamageToDo);
	Health -= DamageToDo;
	if (Health <= 0)
	{
		Died();
	}
	return DamageToDo;
}

void APawnJerry::AddHealth(const float& HealthAdd)
{
	if (Health >= HealthMax)
		return;
	Health += HealthAdd;
	if (Health > HealthMax)
		Health = HealthMax;
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
	if (IsDead())
		return;
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
			//Destroy the weapon the Weapon pointer is currently pointing to
			if (Weapon != nullptr)
			{
				Weapon->StopFire();
				Weapon->Destroy();
			}
			//Spawn an instance of the selected Weapon
			Weapon = GetWorld()->SpawnActor<AWeapon>(wClass);
			if (Weapon)
			{
				Weapon->SetOwner(this);
				if (AmmoComponent != nullptr)
					Weapon->SetAmmoComponent(AmmoComponent);
				Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
				if (bAmplified)
				{
					ASkill_Amplify* Amplify = Cast<ASkill_Amplify>(Skill);
					if (Amplify != nullptr)
					{
						Weapon->SetFireRate(Weapon->GetFireRate() * Amplify->GetFireRateMultiplier());
						Weapon->SetAmmoPerFire(0);
					}
				}
				return;	
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Player does not have this weapon in inventory."));
}

bool APawnJerry::HasWeapon(const int& WeaponNum) const
{
	for (auto wClass: WeaponInventory)
	{
		if (wClass->GetDefaultObject<AWeapon>()->GetWeaponNumber() == WeaponNum)
			return true;
	}
	return false;
}

int APawnJerry::GetSelectedWeapon() const
{
	if (Weapon == nullptr)
		return -1;
	return Weapon->GetWeaponNumber();
}

//Called when a player clicks with left mouse
//Input binding is used here, as SetupPlayerInputComponent is called before BeginPlay and Weapon will be null
void APawnJerry::BeginFire()
{
	if (IsDead())
		return;
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

bool APawnJerry::IsFiringMelee() const
{
	if (Weapon == nullptr)
		return false;
	return Weapon->IsMelee() && Weapon->IsFiring();
}

//Called when player walks over a material
//Adds the material to the MaterialInventory array
void APawnJerry::AddWeaponMaterial(AFactory_Material* MaterialFactory)
{
	if (MaterialFactory != nullptr)
	{
		MaterialInventory.Emplace(MaterialFactory);
		//UE_LOG(LogTemp, Warning, TEXT("Added Material %d to Inventory!"), WeaponMaterialNumber);
	}
}

//Called by CraftingPost when player walks into crafting location
void APawnJerry::CollectMaterials()
{
	MaterialInventory.Empty();
}

void APawnJerry::OpenMenu()
{
	AControllerJerry* PC = Cast<AControllerJerry>(GetController());
	if (PC != nullptr)
		PC->OpenMenu();
}

void APawnJerry::AddEnergyHelper()
{
	AddEnergy(EnergyRegenAmount);
}

void APawnJerry::AddEnergy(const int& Amount)
{
	Energy += FMath::Min(Amount, EnergyMax - Energy);
	UE_LOG(LogTemp, Warning, TEXT("Energy: %d"), Energy);
}

void APawnJerry::NextSkill()
{
	SkillIndex++;
	if (SkillIndex >= Skills.Num())
		SkillIndex = 0;
	UE_LOG(LogTemp, Warning, TEXT("SkillIndex: %d"), SkillIndex);
}

void APawnJerry::PreviousSkill()
{
	SkillIndex--;
	if (SkillIndex < 0)
		SkillIndex = Skills.Num() - 1;
	UE_LOG(LogTemp, Warning, TEXT("SkillIndex: %d"), SkillIndex);
}

void APawnJerry::ExecuteSkill()
{
	if (Skill != nullptr)	//Already have a running skill
	{
		UE_LOG(LogTemp, Warning, TEXT("Already have a skill running!"));
		return;
	}
	if (Energy < EnergyMax)
		return;
	if (Skills.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CAUTION: Skills array is empty for PawnJerry!"));
		return;
	}
	Skill = GetWorld()->SpawnActor<ASkill>(Skills[SkillIndex]);
	if (Skill == nullptr)
		return;
	Skill->SetOwner(this);
	Skill->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);
	Skill->Execute();
	Energy = 0;	//Could use skill energy cost instead, but for player let's just reset energy to 0
}

void APawnJerry::Died()
{
	UE_LOG(LogTemp, Warning, TEXT("Player died!"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Weapon != nullptr)
		Weapon->Destroy();
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	Game->DecrementNumLives();

	//Iterate over Materials array, and call Spawn on the factories
	for (auto Factory : MaterialInventory)
	{
		Factory->SpawnMaterial();
	}

	// TODO: Temporary code. If NumLives is 0, display a game over screen and exit back to the main menu.
	//		 For right now, the game is just restarted.
	if (Game->GetNumLives() == 0) {
		Game->Restart(this);
	}

	float SpawnDelay;
	if (Game != nullptr)
		SpawnDelay = Game->GetPlayerSpawnDelay();
	else
		SpawnDelay = 3.0;
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &APawnJerry::DestroyHelper, SpawnDelay, false, SpawnDelay);
}

void APawnJerry::DestroyHelper()
{
	GetWorldTimerManager().ClearTimer(DestroyTimer);
	Destroy();
}

void APawnJerry::Destroyed()
{
	Super::Destroyed();
	AGameModeBase* Game = GetWorld()->GetAuthGameMode();
	if (Game != nullptr)
	{
		Game->RestartPlayer(GetWorld()->GetFirstPlayerController());
		UE_LOG(LogTemp, Warning, TEXT("Restarting player!"));
	}
}