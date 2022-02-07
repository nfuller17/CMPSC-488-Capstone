// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnJerry.generated.h"

UCLASS()
class TOMANDJERRY_API APawnJerry : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APawnJerry();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Allows the character to move forward(AxisValue = 1.0) or backward(AxisValue = -1.0)
	//Already framerate independent
	void MoveForward(float AxisValue);

	//Allows the character to move rightward(AxisValue = 1.0) or leftward(AxisValue = -1.0)
	//Already framerate independent
	void Strafe(float AxisValue);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PawnJerry.generated.h"

UCLASS()
class TOMANDJERRY_API APawnJerry : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APawnJerry();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Allows the character to move forward(AxisValue = 1.0) or backward(AxisValue = -1.0)
	//Already framerate independent
	void MoveForward(float AxisValue);

	//Allows the character to move rightward(AxisValue = 1.0) or leftward(AxisValue = -1.0)
	//Already framerate independent
	void Strafe(float AxisValue);

};

