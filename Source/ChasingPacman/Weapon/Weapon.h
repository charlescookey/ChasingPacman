// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class CHASINGPACMAN_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	virtual void Tick(float DeltaTime) override;
	virtual void Fire();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, Category = Combat)
		float FireDelay = .15f;

	UPROPERTY(EditAnywhere, Category = Combat)
		bool bAutomatic = true;

	void SetHUDAmmo();

	void SetWeaponState(EWeaponState State);
	bool IsEmpty();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UStaticMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Ammo)
	int32 Ammo;

	UPROPERTY(EditAnywhere)
		int32 MagCapacity;

	void SpendRound();

	UFUNCTION()
	void OnRep_Ammo();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Pawn Components")
	USceneComponent* ProjectileSpawnPoint;
	

	

};
