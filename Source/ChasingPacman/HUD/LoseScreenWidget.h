// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoseScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API ULoseScreenWidget : public UUserWidget
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
