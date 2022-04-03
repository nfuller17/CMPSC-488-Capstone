// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnJerrySpectator.h"
#include "../TomAndJerryGameModeBase.h"

void APawnJerrySpectator::BeginPlay()
{
	Super::BeginPlay();
	CurrentViewTarget = this;
}

void APawnJerrySpectator::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APawnJerrySpectator::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("Strafe"), this, &APawnJerrySpectator::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAction(TEXT("SpectateNextTarget"), EInputEvent::IE_Pressed, this, &APawnJerrySpectator::SpectateNextTarget);
}

void APawnJerrySpectator::MoveForward(float Val)
{
	Super::MoveForward(Val);
	AddMovementInput(GetActorForwardVector() * Val);
}

void APawnJerrySpectator::MoveRight(float Val)
{
	Super::MoveRight(Val);
	AddMovementInput(GetActorRightVector() * Val);
}

void APawnJerrySpectator::SpectateNextTarget()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC == nullptr)
		return;
	ATomAndJerryGameModeBase* Game = Cast<ATomAndJerryGameModeBase>(GetWorld()->GetAuthGameMode());
	if (Game == nullptr)
		return;
	if (Game->GetSpectateMode())
	{
		for (int x = 0; Game != nullptr && Game->SpectateList.Num() > 0 && x < Game->SpectateList.Num(); x++)
		{
			if (CurrentViewTarget != nullptr)
			{
				if (CurrentViewTarget == this)
				{
					PC->SetViewTarget(Game->SpectateList[x]);
					CurrentViewTarget = Game->SpectateList[x];
					return;
				}
				else if (CurrentViewTarget == Game->SpectateList[x])
				{
					//Get next one in list
					if (x + 1 < Game->SpectateList.Num())
					{
						PC->SetViewTarget(Game->SpectateList[x + 1]);
						CurrentViewTarget = Game->SpectateList[x + 1];
						return;
					}
					else if (x == Game->SpectateList.Num() - 1)	//At last in list, go to beginning
					{
						PC->SetViewTarget(Game->SpectateList[0]);
						CurrentViewTarget = Game->SpectateList[0];
						return;
					}
				}
			}
		}
	}
	/*APawn* CurrentViewTarget = Cast<APawn>(PC->GetViewTarget());
	for (auto Target : TActorRange<APawn>(GetWorld()))
	{
		if (CurrentViewTarget != nullptr && CurrentViewTarget == Target)
		{
			continue;
		}
		PC->AutoManageActiveCameraTarget(Target);
		return;
	}
	*/
}