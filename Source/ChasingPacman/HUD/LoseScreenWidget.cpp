// Fill out your copyright notice in the Description page of Project Settings.


#include "LoseScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void ULoseScreenWidget::MainMenuButtonClicked() {
	UGameplayStatics::OpenLevel(GetWorld(), FName("Startup"));
}



void ULoseScreenWidget::NextLevelButtonClicked() {
	UWorld* World = GetWorld();
	if (World)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found world from here lool"));
		//bUseSeamlessTravel = true;
		World->ServerTravel(FString("/Game/Maps/Lobby?listen"));
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