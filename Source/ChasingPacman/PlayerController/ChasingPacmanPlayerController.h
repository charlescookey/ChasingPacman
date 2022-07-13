// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChasingPacmanPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API AChasingPacmanPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetHUDHealth(float Health, float MaxHealth);
	
	virtual void OnPossess(APawn* InPawn) override;

	void SetPacmanLives(int32 LivesLeft);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetEndGame();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ATankHUD* TankHUD;

	UPROPERTY(ReplicatedUsing = OnRep_EndScreen)
	bool bShouldShowEndScreen = false;
	
	UPROPERTY(ReplicatedUsing = OnRep_PacmanLives)
	int32 PacmanLives = 4;

	UFUNCTION()
	void OnRep_EndScreen();

	UFUNCTION()
	void OnRep_PacmanLives();


	
};
