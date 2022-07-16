// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TankHUD.generated.h"


USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
public:
	class UTexture2D* CrosshairsCenter;
	UTexture2D* CrosshairsLeft;
	UTexture2D* CrosshairsRight;
	float CrosshairSpread;
	FLinearColor CrosshairsColor;
};

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API ATankHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> TankOverlayClass;
	
	class UTankOverlay* TankOverlay;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> EndGameClass;

	class ULoseScreenWidget* EndGame;

	void AddEndGame();
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<class UUserWidget> WinGameClass;

	class UWinScreenWidget* WinGame;

	void AddWinGame();

protected:
	virtual void BeginPlay() override;
	void AddTankOverlay();

	void SetPacmanLives();

private:
	FHUDPackage HUDPackage;
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);

	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
};
