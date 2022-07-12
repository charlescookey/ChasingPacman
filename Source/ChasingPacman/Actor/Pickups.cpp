// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"
#include "ChasingPacman/Character/PacmanCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"

// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	SetRootComponent(PickupMesh);


	PickupMesh->SetCollisionResponseToAllChannels(ECR_Overlap);

}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APickups::OnOverlap);
	
}

// Called every frame
void APickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickups::OnOverlap(AActor* OverlappedActor, AActor* OtherActor) {
	APacmanCharacter* pac = Cast<APacmanCharacter>(OtherActor);
	if (pac) {
		UE_LOG(LogTemp, Error, TEXT("This actor %s overlappeedby pacman"), *OverlappedActor->GetName());
		if (IsPowerUp) {
			pac->PoweredUp(OverlappedActor);
			UE_LOG(LogTemp, Error, TEXT("Pacman powered up"));
		}else {
			pac->PickUpEaten(OverlappedActor);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("This actor %s overlappeed by %s not pacman"), *OverlappedActor->GetName() , *OtherActor->GetName());
	}
}

void APickups::Destroyed()
{
	Super::Destroyed();

	if (CaptureParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CaptureParticles, GetActorTransform());
	}
	if (CaptureSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CaptureSound, GetActorLocation());
	}
}

