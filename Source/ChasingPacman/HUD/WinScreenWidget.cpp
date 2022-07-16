// Fill out your copyright notice in the Description page of Project Settings.


#include "WinScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UWinScreenWidget::MainMenuButtonClicked() {
	UGameplayStatics::OpenLevel(GetWorld(), FName("Startup"));
}



void UWinScreenWidget::NextLevelButtonClicked() {
	UWorld* World = GetWorld();
	if (World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found world from here lool"));
		//bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
	}

}

bool UWinScreenWidget::Initialize() {
	if (!Super::Initialize())
	{
		return false;
	}

	if (MainMenuButton)
	{
		MainMenuButton->OnClicked.AddDynamic(this, &UWinScreenWidget::MainMenuButtonClicked);
	}
	if (NextLevelButton)
	{
		NextLevelButton->OnClicked.AddDynamic(this, &UWinScreenWidget::NextLevelButtonClicked);
	}

	return true;
}
