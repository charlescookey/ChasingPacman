// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class CHASINGPACMAN_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pawn Components", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pawn Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pawn Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pawn Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

};
