// Fill out your copyright notice in the Description page of Project Settings.


#include "TankHUD.h"
#include "GameFramework/PlayerController.h"
#include "TankOverlay.h"

#include "ChasingPacman/HUD/LoseScreenWidget.h"
#include "ChasingPacman/HUD/WinScreenWidget.h"

#include"ChasingPacman/PlayerController/ChasingPacmanPlayerController.h"


void ATankHUD::BeginPlay()
{
	Super::BeginPlay();

	AddTankOverlay();

	UE_LOG(LogTemp, Display, TEXT("I want to see when this will be called"));
	
	SetPacmanLives();

}

void ATankHUD::SetPacmanLives() {
	AChasingPacmanPlayerController* PlayerController = Cast<AChasingPacmanPlayerController>(PlayerOwner);
	UE_LOG(LogTemp, Display, TEXT("trying to set pacman lives"));
	if (PlayerController) {
		UE_LOG(LogTemp, Display, TEXT("PlayerOwner was castable"));
		PlayerController->SetPacmanLives(PlayerController->GetPacmanLives());
	}
}

void ATankHUD::AddTankOverlay()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && TankOverlayClass)
	{
		TankOverlay = CreateWidget<UTankOverlay>(PlayerController, TankOverlayClass);
		TankOverlay->AddToViewport();
	}
}

void ATankHUD::AddEndGame()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && EndGameClass)
	{
		EndGame = CreateWidget<ULoseScreenWidget>(PlayerController, EndGameClass);
		EndGame->AddToViewport();
	}
}

void ATankHUD::AddWinGame()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && WinGameClass)
	{
		WinGame = CreateWidget<UWinScreenWidget>(PlayerController, WinGameClass);
		WinGame->AddToViewport();
	}
}

void ATankHUD::DrawHUD()
{
	Super::DrawHUD();
	FVector2D ViewportSize;
	if (GEngine)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 20.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 20.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 20.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
}

void ATankHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);
	//UE_LOG(LogTemp, Error, TEXT("Texture : %s"), *Texture->GetName());
	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
	);
}