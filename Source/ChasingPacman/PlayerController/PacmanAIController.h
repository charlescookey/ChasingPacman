// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PacmanAIController.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API APacmanAIController : public AAIController
{
	GENERATED_BODY()
public:
	void UpdateList(AActor* , FVector);

	void UpdateNearestPickUp(FVector);

	void UpdateNearestActor(FVector);

	FVector NearestActor();

	void Powered(AActor* , FVector);

	void UnPowered(FVector);


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;


	TArray<AActor*> Picksups;
	TArray<AActor*> Players;


	bool IsPowered = false;
	
	UPROPERTY()
	ACharacter* MyCharacter;
};
