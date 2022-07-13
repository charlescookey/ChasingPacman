// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PacmanCharacter.generated.h"

UCLASS()
class CHASINGPACMAN_API APacmanCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacmanCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void PickUpEaten(AActor* PickUp);

	void PoweredUp(AActor* PickUp);

	void Died();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDied();



private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pawn Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, Category = "Tank Components")
		float MoveSpeed = 250.f;

	UPROPERTY(EditAnywhere, Category = "Tank Components")
		float TurnSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWidgetComponent* PacmanHealthWidget;

	UPROPERTY(EditAnywhere, Category = "Player Stats")
		class UPacmanHealthWidget* HealthWidget;

	UPROPERTY()
	class APacmanAIController* PacmanAIController;

	/**
	* Player health
	*/

	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health();

	void UpdateHealthHUD();


	//color changing
	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* YellowMaterialInstance;

	UPROPERTY(EditAnywhere, Category = Elim)
	UMaterialInstance* BlueMaterialInstance;

	UPROPERTY(ReplicatedUsing = OnRep_PoweredUpState)
	bool IsPoweredUp = false;

	UPROPERTY(EditAnywhere)
	int32 PowerUpTimerLenght = 30;

	UFUNCTION()
	void OnRep_PoweredUpState();

	FTimerHandle PowerUpTimer;

	void PowerupTimerFinished();

	//Death

	FTimerHandle DeathTimer;

	UPROPERTY(EditDefaultsOnly)
		float DeathDelay = 3.f;

	void DeathTimerFinished();

	bool alive = true;
};
