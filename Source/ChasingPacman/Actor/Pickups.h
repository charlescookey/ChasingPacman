// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickups.generated.h"

UCLASS()
class CHASINGPACMAN_API APickups : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickups();
	virtual void Destroyed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Pickup Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PickupMesh;

	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	UParticleSystem* CaptureParticles;

	UPROPERTY(EditAnywhere)
	class USoundCue* CaptureSound;

	UPROPERTY(EditAnywhere)
	bool IsPowerUp = false;

};
