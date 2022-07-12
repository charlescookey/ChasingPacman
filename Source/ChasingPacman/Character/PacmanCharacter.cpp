// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanCharacter.h"
#include "Components/CapsuleComponent.h"
#include "ChasingPacman/ChasingPacman.h"
#include "ChasingPacman/HUD/PacmanHealthWidget.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "ChasingPacman/PlayerController/PacmanAIController.h"
#include "ChasingPacman/PlayerController/ChasingPacmanPlayerController.h"
#include "ChasingPacman/GameMode/ChasingPacmanGameMode.h"
#include "ChasingPacman/Actor/Pickups.h"
#include "Kismet/GameplayStatics.h"
#include "ChasingPacman/Character/BaseCharacter.h"

// Sets default values
APacmanCharacter::APacmanCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pacman Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	BaseMesh->SetCollisionObjectType(ECC_StaticMesh);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	PacmanHealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	PacmanHealthWidget->SetupAttachment(RootComponent);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

// Called when the game starts or when spawned
void APacmanCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &APacmanCharacter::ReceiveDamage);

		if (GetCapsuleComponent()) {
			GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &APacmanCharacter::OnHit);
		}
	}

	UpdateHealthHUD();


	PacmanAIController = PacmanAIController == nullptr ? Cast<APacmanAIController>(Controller) : PacmanAIController;

	if (PacmanAIController) {
		UE_LOG(LogTemp, Warning, TEXT("pacman ai controller is active"));
		PacmanAIController->UpdateNearestPickUp(GetActorLocation());
	}

	alive = true;
	
}

// Called every frame
void APacmanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APacmanCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APacmanCharacter, Health);
	DOREPLIFETIME(APacmanCharacter, IsPoweredUp);

}

void APacmanCharacter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && IsPoweredUp) {
		ABaseCharacter* TankCharacter = Cast<ABaseCharacter>(OtherActor);
		if (TankCharacter && TankCharacter->alive) {
				UGameplayStatics::ApplyDamage(OtherActor, TankCharacter->MaxDamage, Controller, this, UDamageType::StaticClass());
		}
	}


}

// Called to bind functionality to input
void APacmanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APacmanCharacter::Died() {
	MulticastDied();
	GetWorldTimerManager().SetTimer(DeathTimer, this, &APacmanCharacter::DeathTimerFinished, DeathDelay);
}

void APacmanCharacter::MulticastDied_Implementation() {
	//effect
}

void APacmanCharacter::DeathTimerFinished() {
	AChasingPacmanGameMode* PacmanGameMode = GetWorld()->GetAuthGameMode<AChasingPacmanGameMode>();
	if (PacmanGameMode) {
		PacmanGameMode->RespawnPacman(this);
	}
}

void APacmanCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	if (!alive)return;
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Applied damage to pacman, Health : %f") , Health);
	UpdateHealthHUD();

	if (Health == 0.f) {
		alive = false;
		UE_LOG(LogTemp, Error, TEXT("Pacman Is Dead"));
		AChasingPacmanGameMode* PacmanGameMode = GetWorld()->GetAuthGameMode<AChasingPacmanGameMode>();
		if (PacmanGameMode) {
			PacmanAIController = PacmanAIController == nullptr ? Cast<APacmanAIController>(Controller) : PacmanAIController;
			AChasingPacmanPlayerController* AttackController = Cast<AChasingPacmanPlayerController>(InstigatorController);

			PacmanGameMode->PacmanDied(this, PacmanAIController, AttackController);
		}

	}
}

void APacmanCharacter::UpdateHealthHUD() {
	HealthWidget = HealthWidget == nullptr? Cast<UPacmanHealthWidget>(PacmanHealthWidget->GetUserWidgetObject()) : HealthWidget;
	if (HealthWidget) {
		HealthWidget->SetHUDHealth(Health, MaxHealth);
	}
};

void APacmanCharacter::OnRep_Health()
{
	UpdateHealthHUD();
	//see if the matinee shake is available
}

void APacmanCharacter::PickUpEaten(AActor* PickUp) {
	if (!HasAuthority())return;
	PacmanAIController = PacmanAIController == nullptr ? Cast<APacmanAIController>(Controller) : PacmanAIController;

	if (PacmanAIController) {
		PacmanAIController->UpdateList(PickUp, GetActorLocation());
	}
};


void APacmanCharacter::PoweredUp(AActor* PickUp) {
	
	//try to ensure tthat when we're powered up we can't get shot at, we can chnage the health bar to black so that wy we don't kill and alsoo don't
	//take damge while w;re powered
	if (!HasAuthority())return;
	IsPoweredUp = true;
	if (BaseMesh && BlueMaterialInstance) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to change the Material Instance"));
		BaseMesh->SetMaterial(0, BlueMaterialInstance);
	}
	GetWorldTimerManager().SetTimer(PowerUpTimer, this, &APacmanCharacter::PowerupTimerFinished, PowerUpTimerLenght);
	
	PacmanAIController = PacmanAIController == nullptr ? Cast<APacmanAIController>(Controller) : PacmanAIController;

	if (PacmanAIController) {
		PacmanAIController->Powered(PickUp ,GetActorLocation());
	}
}

void APacmanCharacter::PowerupTimerFinished() {
	IsPoweredUp = false;
	if (BaseMesh && YellowMaterialInstance) {
		BaseMesh->SetMaterial(0, YellowMaterialInstance);
	}

	PacmanAIController = PacmanAIController == nullptr ? Cast<APacmanAIController>(Controller) : PacmanAIController;

	if (PacmanAIController) {
		PacmanAIController->UnPowered(GetActorLocation());
	}
}

void APacmanCharacter::OnRep_PoweredUpState() {
	if (IsPoweredUp) {
		if (BaseMesh && BlueMaterialInstance) {
			BaseMesh->SetMaterial(0, BlueMaterialInstance);
		}
	}
	else {
		if (BaseMesh && YellowMaterialInstance) {
			BaseMesh->SetMaterial(0, YellowMaterialInstance);
		}
	}
}
