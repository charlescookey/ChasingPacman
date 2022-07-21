// Fill out your copyright notice in the Description page of Project Settings.


#include "LoseScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include "ChasingPacman/GameMode/ChasingPacmanGameMode.h"

void ULoseScreenWidget::MainMenuButtonClicked() {
	UGameplayStatics::OpenLevel(GetWorld(), FName("Startup"));
}



void ULoseScreenWidget::NextLevelButtonClicked() {
	UWorld* World = GetWorld();
	if (World)
	{
		AChasingPacmanGameMode* GameMode = World->GetAuthGameMode< AChasingPacmanGameMode>();

		if (GameMode) {
			GameMode->RestartGame();
		}
	}
}

bool ULoseScreenWidget::Initialize() {
	if (!Super::Initialize())
	{
		return false;
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &ULoseScreenWidget::MainMenuButtonClicked);
	}
	if (NextLevelButton)
	{
		NextLevelButton->OnClicked.AddDynamic(this, &ULoseScreenWidget::NextLevelButtonClicked);
	}

	return true;
}