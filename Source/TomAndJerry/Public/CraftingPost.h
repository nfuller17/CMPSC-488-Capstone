// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CraftingPost.generated.h"

UCLASS()
class TOMANDJERRY_API ACraftingPost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingPost();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Called when Player pawn walks into the Crafting Post
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		UParticleSystem* CollectEffect;
};
