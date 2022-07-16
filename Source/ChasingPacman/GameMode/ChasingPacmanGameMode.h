// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ChasingPacmanGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API AChasingPacmanGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void PlayerEliminated(class ABaseCharacter* ElimmedCharacter, class AChasingPacmanPlayerController* VictimController, AController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	virtual void RespawnPacman(class APacmanCharacter* DeadPacman);

	virtual void PacmanDied(class APacmanCharacter* ElimmedCharacter, class APacmanAIController* PacmanController, AChasingPacmanPlayerController* AttackerController);

	void EndGame();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APacmanCharacter> PacmanClass;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	int32 PacmanLives = 3;
public:

	FORCEINLINE int32 GetPacmanLives() const { return PacmanLives; }

};
