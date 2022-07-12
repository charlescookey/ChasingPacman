// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	APlayerController* GetTankPlayerControllerRef()const { return PlayerControllerRef; };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Tank Components")
		class UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Tank Components")
		class USpringArmComponent* SpringArm = nullptr;

	void Move(float Value);
	void Turn(float Value);


	UPROPERTY(EditAnywhere, Category = "Tank Components")
		float MoveSpeed = 250.f;

	UPROPERTY(EditAnywhere, Category = "Tank Components")
		float TurnSpeed = 100.f;

	APlayerController* PlayerControllerRef;




	
};
