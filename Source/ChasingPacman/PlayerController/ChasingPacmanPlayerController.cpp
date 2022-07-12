// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasingPacmanPlayerController.h"
#include "ChasingPacman/HUD/TankHUD.h"
#include "ChasingPacman/HUD/TankOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Net/UnrealNetwork.h"

void AChasingPacmanPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankHUD = Cast<ATankHUD>(GetHUD());
}

void AChasingPacmanPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	bool bHUDValid = TankHUD &&
		TankHUD->TankOverlay &&
		TankHUD->TankOverlay->HealthBar &&
		TankHUD->TankOverlay->HealthText;

	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		TankHUD->TankOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		TankHUD->TankOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void AChasingPacmanPlayerController::SetEndGame() {
	bShouldShowEndScreen = true;
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	DisableInput(this);

	if (TankHUD) {
		TankHUD->AddEndGame();
	}
}


void AChasingPacmanPlayerController::OnRep_EndScreen() {
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	DisableInput(this);

	if (TankHUD) {
		TankHUD->AddEndGame();
	}
}

void AChasingPacmanPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AChasingPacmanPlayerController, bShouldShowEndScreen);
	DOREPLIFETIME(AChasingPacmanPlayerController, PacmanLives);
}


void AChasingPacmanPlayerController::SetPacmanLives(int32 LivesLeft) {
	PacmanLives = LivesLeft;
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	if (PacmanLives == 0) {
		if (TankHUD) {
			UE_LOG(LogTemp, Warning, TEXT("We're setting Pacman lives"));
			TankHUD->AddWinGame();
		}
		return;
	}

	if (TankHUD && TankHUD->TankOverlay && TankHUD->TankOverlay->PacmanLives) {
		FString PacmanLivesText = FString::Printf(TEXT("%d"), PacmanLives);
	}
}

void AChasingPacmanPlayerController::OnRep_PacmanLives() {
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;
	
	if (PacmanLives == 0) {
		if (TankHUD) {
			TankHUD->AddWinGame();
		}
		return;
	}

	if (TankHUD && TankHUD->TankOverlay && TankHUD->TankOverlay->PacmanLives) {
		FString PacmanLivesText = FString::Printf(TEXT("%d"), PacmanLives);
	}
}