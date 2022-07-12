// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank() {
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("The Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("The Camera Conmponent"));
    CameraComponent->SetupAttachment(SpringArm);
};

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
    Super::BeginPlay();
    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerControllerRef) {
        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

        RotateTurret(HitResult.ImpactPoint);
    }

}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float Value) {
    FVector DeltaLoaction = FVector::ZeroVector;
    DeltaLoaction.X = Value * MoveSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLoaction, true);
    //AddActorLocalOffset(FVector(Value , 0.f ,0.f) );
}

void ATank::Turn(float Value) {
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Value * TurnSpeed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
}
