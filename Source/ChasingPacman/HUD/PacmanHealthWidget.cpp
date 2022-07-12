// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanHealthWidget.h"
#include "Components/ProgressBar.h"

void UPacmanHealthWidget::SetHUDHealth(float Health, float MaxHealth)
{
	if (HealthBar)
	{
		const float HealthPercent = Health / MaxHealth;
		HealthBar->SetPercent(HealthPercent);
	}
}
void UPacmanHealthWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}