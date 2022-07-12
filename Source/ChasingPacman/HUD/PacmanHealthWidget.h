// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PacmanHealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API UPacmanHealthWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	void SetHUDHealth(float Health, float MaxHealth);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
