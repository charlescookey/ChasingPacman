// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasingPacmanPlayerController.h"
#include "ChasingPacman/HUD/TankHUD.h"
#include "ChasingPacman/HUD/TankOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#include "Net/UnrealNetwork.h"
#include "ChasingPacman/Character/BaseCharacter.h"
#include "ChasingPacman/GameMode/ChasingPacmanGameMode.h"

void AChasingPacmanPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TankHUD = Cast<ATankHUD>(GetHUD());
	UE_LOG(LogTemp, Warning, TEXT("Begin play in rthe controller"));
}

void AChasingPacmanPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(InPawn);
	if (BaseCharacter)
	{
		SetHUDHealth(BaseCharacter->GetHealth(), BaseCharacter->GetMaxHealth());
	}

	UE_LOG(LogTemp, Warning, TEXT("Tried setting the lives text"));
	AChasingPacmanGameMode* PacmanGameMode = GetWorld()->GetAuthGameMode<AChasingPacmanGameMode>();
	if (PacmanGameMode) {
		UE_LOG(LogTemp, Warning, TEXT("entered the acc try Tried setting the lives text"));
		SetPacmanLives(PacmanGameMode->GetPacmanLives());
	}
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
		UE_LOG(LogTemp, Warning, TEXT("Should appear att the beginning lool"));
	}
}

void AChasingPacmanPlayerController::SetEndGame() {
	bShouldShowEndScreen = true;
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	ABaseCharacter* MyCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (MyCharacter) {
		MyCharacter->Stop();
	}

	if (TankHUD) {
		TankHUD->AddEndGame();
	}
}


void AChasingPacmanPlayerController::OnRep_EndScreen() {
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	ABaseCharacter* MyCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (MyCharacter) {
		MyCharacter->Stop();
	}

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
	UE_LOG(LogTemp, Warning, TEXT("Lives left: %i") , LivesLeft);
	PacmanLives = LivesLeft;
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;

	if (PacmanLives == 0) {
		if (TankHUD) {
			
			TankHUD->AddWinGame();
		}
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Pacman Lives left: %i"), PacmanLives);

	if (TankHUD && TankHUD->TankOverlay && TankHUD->TankOverlay->PacmanLives) {
		UE_LOG(LogTemp, Warning, TEXT("We're setting Pacman lives : %d"), PacmanLives);
		FString PacmanLivesText = FString::Printf(TEXT("%d"), PacmanLives);
		TankHUD->TankOverlay->PacmanLives->SetText(FText::FromString(PacmanLivesText));
	}
}

void AChasingPacmanPlayerController::OnRep_PacmanLives() {
	TankHUD = TankHUD == nullptr ? Cast<ATankHUD>(GetHUD()) : TankHUD;
	UE_LOG(LogTemp, Warning, TEXT("rep Lives left: %i"), PacmanLives);
	if (PacmanLives == 0) {
		if (TankHUD) {
			TankHUD->AddWinGame();
		}
		return;
	}

	if (TankHUD && TankHUD->TankOverlay && TankHUD->TankOverlay->PacmanLives) {
		UE_LOG(LogTemp, Warning, TEXT("We're setting Pacman lives : %d"), PacmanLives);
		FString PacmanLivesText = FString::Printf(TEXT("%d"), PacmanLives);
		TankHUD->TankOverlay->PacmanLives->SetText(FText::FromString(PacmanLivesText));
	}
}