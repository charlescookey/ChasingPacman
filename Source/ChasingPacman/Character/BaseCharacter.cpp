// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ChasingPacman/Projectile/Projectile.h"
#include "Components/CapsuleComponent.h"
#include "ChasingPacman/ChasingPacman.h"
#include "Net/UnrealNetwork.h"
#include "ChasingPacman/PlayerController/ChasingPacmanPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "ChasingPacman/GameMode/ChasingPacmanGameMode.h"
#include "Sound/SoundCue.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);//, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Component"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	
	BaseMesh->SetCollisionObjectType(ECC_StaticMesh);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BaseMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	TurretMesh->SetCollisionObjectType(ECC_StaticMesh);
	TurretMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	TurretMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	ActiveController = false;
	Super::BeginPlay();
	Health = MaxHealth;
	UpdateHUDHealth();

	alive = true;
	if (HasAuthority())
	{
		OnTakeAnyDamage.AddDynamic(this, &ABaseCharacter::ReceiveDamage);
	}
}

void ABaseCharacter::SetHUDOnceControllerIsActive() {
	if (!ActiveController) {
		UpdateHUDHealth();
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsLocallyControlled())
	{
		//FHitResult HitResult;
		//TraceUnderCrosshairs(HitResult);
		//HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		SetHUDOnceControllerIsActive();
	}

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABaseCharacter, Ammo);
	DOREPLIFETIME(ABaseCharacter, Health);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("RotateTurret"), this, &ABaseCharacter::RotateTurret);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::FireButtonReleased);

}



void ABaseCharacter::RotateTurret(float Value) {
	AddControllerYawInput(Value);
}


void ABaseCharacter::Move(float Value) {
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::Turn(float Value) {
	/*float TurnValue = Value * TurnSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddControllerYawInput(TurnValue);*/

	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void ABaseCharacter::FireButtonPressed()
{
	FireButtonPressedDown(true);
}

void ABaseCharacter::FireButtonReleased()
{
	FireButtonPressedDown(false);
}

void ABaseCharacter::FireButtonPressedDown(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if (bFireButtonPressed)
	{
		Fire();
	}

}

void ABaseCharacter::Fire()
{
	if (CanFire())
	{
		bCanFire = false;
		ServerFire();
		CrosshairShootingFactor = .75f;
		StartFireTimer();
	}
}

void ABaseCharacter::StartFireTimer()
{
	GetWorldTimerManager().SetTimer(
		FireTimer,
		this,
		&ABaseCharacter::FireTimerFinished,
		FireDelay
	);
}

void ABaseCharacter::FireTimerFinished()
{
	bCanFire = true;
	if (bFireButtonPressed && bAutomatic)
	{
		Fire();
	}
}

void ABaseCharacter::ServerFire_Implementation()
{
	MulticastFire();
}

void ABaseCharacter::MulticastFire_Implementation()
{
	Shoot();
}

bool ABaseCharacter::CanFire()
{
	return !IsEmpty() || !bCanFire;
}

void ABaseCharacter::Eliminated() {
	UE_LOG(LogTemp, Warning, TEXT("This actor was elimianted"));
	MulticastEliminated();
	GetWorldTimerManager().SetTimer(ElimantionTimer, this, &ABaseCharacter::EliminationTimerFinished, EliminationDelay);
}

void ABaseCharacter::EliminationTimerFinished() {
	UE_LOG(LogTemp, Error, TEXT("Elim Timer Finished"));
	AChasingPacmanGameMode* PacmanGameMode = GetWorld()->GetAuthGameMode<AChasingPacmanGameMode>();
	if (PacmanGameMode) {
		PacmanGameMode->RequestRespawn(this, Controller);
	}
}

void ABaseCharacter::MulticastEliminated_Implementation() {
	// Spawn elim bot
	if (ElimBotEffect)
	{
		FVector ElimBotSpawnPoint(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f);
		ElimBotComponent = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ElimBotEffect,
			ElimBotSpawnPoint,
			GetActorRotation()
		);
	}
	if (ElimBotSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			this,
			ElimBotSound,
			GetActorLocation()
		);
	}
}

void ABaseCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHUDHealth();
	UE_LOG(LogTemp, Warning, TEXT("Health : %f") , Health);
	//see if matinee shake is available


	ShakeCamera();
	//add elimination once we add player statte

	if (Health == 0.f) {
		alive = false;
		UE_LOG(LogTemp, Error, TEXT("Player Is Dead"));
		AChasingPacmanGameMode* PacmanGameMode = GetWorld()->GetAuthGameMode<AChasingPacmanGameMode>();
		if (PacmanGameMode) {
			UE_LOG(LogTemp, Error, TEXT("Pacman gamemode active"));
			ChasingPacmanPlayerController = ChasingPacmanPlayerController == nullptr ? Cast<AChasingPacmanPlayerController>(Controller) : ChasingPacmanPlayerController;

			PacmanGameMode->PlayerEliminated(this, ChasingPacmanPlayerController, InstigatorController);
		}
	}
}

void ABaseCharacter::OnRep_Health()
{
	UpdateHUDHealth();
	//see if the matinee shake is available

	ShakeCamera();
}

void ABaseCharacter::ShakeCamera() {
	UE_LOG(LogTemp, Error, TEXT("Rep - Is Locally COntrolled?"));
	if (IsLocallyControlled()) {
		ChasingPacmanPlayerController = ChasingPacmanPlayerController == nullptr ? Cast<AChasingPacmanPlayerController>(Controller) : ChasingPacmanPlayerController;
		UE_LOG(LogTemp, Error, TEXT("rep - Yes, It is Locally COntrolled"));
		if (ChasingPacmanPlayerController && HitCameraShakeClass) {
			ChasingPacmanPlayerController->ClientStartCameraShake(HitCameraShakeClass);
		}
	}
}

void ABaseCharacter::UpdateHUDHealth()
{
	ChasingPacmanPlayerController = ChasingPacmanPlayerController == nullptr ? Cast<AChasingPacmanPlayerController>(Controller) : ChasingPacmanPlayerController;
	if (ChasingPacmanPlayerController)
	{
		ActiveController = true;
		ChasingPacmanPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void ABaseCharacter::SpendRound()
{
	Ammo = FMath::Clamp(Ammo - 1, 0, MagCapacity);
	SetHUDAmmo();
}

void ABaseCharacter::OnRep_Ammo()
{
	SetHUDAmmo();
}

void ABaseCharacter::Shoot()
{
	SpendRound();

	if (!HasAuthority()) return;

	APawn* InstigatorPawn = Cast<APawn>(this);

	if (ProjectileClass && InstigatorPawn)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = InstigatorPawn;
		UWorld* World = GetWorld();
		if (World)
		{
			World->SpawnActor<AProjectile>(
				ProjectileClass,
				ProjectileSpawnPoint->GetComponentLocation(),
				ProjectileSpawnPoint->GetComponentRotation(),
				SpawnParams
				);
		}
	}
}

bool ABaseCharacter::IsEmpty()
{
	return Ammo <= 0;
}

void ABaseCharacter::SetHUDAmmo()
{
}


void ABaseCharacter::SetHUDCrosshairs(float DeltaTime)
{
	if (Controller == nullptr) return;
	ChasingPacmanPlayerController = ChasingPacmanPlayerController == nullptr ? Cast<AChasingPacmanPlayerController>(Controller) : ChasingPacmanPlayerController;
	if (ChasingPacmanPlayerController)
	{

		HUD = HUD == nullptr ? Cast<ATankHUD>(ChasingPacmanPlayerController->GetHUD()) : HUD;
		if (HUD)
		{
			HUDPackage.CrosshairsCenter = CrosshairsCenter;
			HUDPackage.CrosshairsLeft = CrosshairsLeft;
			HUDPackage.CrosshairsRight = CrosshairsRight;

			//color
			HUDPackage.CrosshairsColor = FLinearColor::White;

			// Calculate crosshair spread

			// [0, 600] -> [0, 1]
			FVector2D WalkSpeedRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = GetVelocity();
			Velocity.Z = 0.f;

			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			CrosshairShootingFactor = FMath::FInterpTo(CrosshairShootingFactor, 0.f, DeltaTime, 40.f);

			HUDPackage.CrosshairSpread =
				0.5f +
				CrosshairVelocityFactor +
				CrosshairShootingFactor -
				CrosshairsOpponent;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
}