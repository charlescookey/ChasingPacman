// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API UWinScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;

	UPROPERTY(meta = (BindWidget))
		UButton* NextLevelButton;

	UFUNCTION()
		void MainMenuButtonClicked();

	UFUNCTION()
		void NextLevelButtonClicked();
	
};
