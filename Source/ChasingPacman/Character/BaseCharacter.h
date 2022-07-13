// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChasingPacman/HUD/TankHUD.h"
#include "BaseCharacter.generated.h"

UCLASS()
class CHASINGPACMAN_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	
	void Move(float Value);
	void Turn(float Value);
	void RotateTurret(float Value);
	void FireButtonPressed();
	void FireButtonReleased();
	
	void UpdateHUDHealth();

	void FireButtonPressedDown(bool bPressed);
	void Fire();

	UFUNCTION(Server, Reliable)
		void ServerFire();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastFire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	bool alive = true;
	float MaxDamage = 100.f;

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pawn Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components")
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Tank Components")
		float MoveSpeed = 250.f;

	UPROPERTY(EditAnywhere, Category = "Tank Components")
		float TurnSpeed = 100.f;

	bool bFireButtonPressed;

	UPROPERTY(EditAnywhere , Category = "Tank Components")
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UStaticMeshComponent* TurretMesh;

	bool CanFire();

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();


	void ShakeCamera();


public:
	virtual void Shoot();

	UPROPERTY(EditAnywhere, Category = Combat)
	float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bAutomatic = true;

	bool IsEmpty();

	void SetHUDAmmo();

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		class UTexture2D* CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = Crosshairs)
		UTexture2D* CrosshairsRight;

	void Eliminated();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastEliminated();

	void Stop();


private:
	/**
	* Elim bot
	*/

	UPROPERTY(EditAnywhere)
		UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
		class USoundCue* ElimBotSound;

	FTimerHandle ElimantionTimer;

	UPROPERTY(EditDefaultsOnly)
		float EliminationDelay = 3.f;

	void EliminationTimerFinished();


	/**
	* Ammo
	*/
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
		int32 Ammo;

	UPROPERTY(EditAnywhere)
		int32 MagCapacity;

	void SpendRound();

	UFUNCTION()
		void OnRep_Ammo();

	UPROPERTY()
		class AChasingPacmanPlayerController* ChasingPacmanPlayerController;
	UPROPERTY()
		class ATankHUD* HUD;

	FHUDPackage HUDPackage;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairShootingFactor;
	float CrosshairsOpponent;

	/**
	* Player health
	*/

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	UPROPERTY(EditAnywhere, Category = "Camera Shake")
	TSubclassOf<class UCameraShakeBase> HitCameraShakeClass;



protected:
	void SetHUDCrosshairs(float DeltaTime);

public:
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }


	

	

};
