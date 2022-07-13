// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanAIController.h"
#include "AIController.h"
#include "ChasingPacman/Character/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ChasingPacman/Actor/Pickups.h"
#include "ChasingPacmanPlayerController.h"


#include "ChasingPacman/GameMode/ChasingPacmanGameMode.h"

void APacmanAIController::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APickups::StaticClass(), Picksups);
    UE_LOG(LogTemp, Warning, TEXT("no of pickups in aicontroller world roght now: %i"), Picksups.Num());

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);
    UE_LOG(LogTemp, Warning, TEXT("no of players in aicontroller world roght now: %i"), Players.Num());

    
    if (AIBehavior) {
        RunBehaviorTree(AIBehavior);
    }

}

void APacmanAIController::UpdateNearestPickUp(FVector CurrentLocation){
    FVector MoveLocation;
    double distance = TNumericLimits<double>::Max();
    for (AActor* pickup : Picksups)
    {
        if (pickup) {
            double temp = FVector::Dist(CurrentLocation, pickup->GetActorLocation());
            if (temp < distance) {
                distance = temp;
                MoveLocation = pickup->GetActorLocation();
            }
        }
    }

    GetBlackboardComponent()->SetValueAsVector(TEXT("PickUpLocation"), MoveLocation);

}

void APacmanAIController::UpdateList(AActor* Pickup , FVector CurrentLocation) {
    int removed = Picksups.Remove(Pickup);
    UE_LOG(LogTemp, Warning, TEXT("no of pickups deleted: %i"), removed);
    UE_LOG(LogTemp, Warning, TEXT("no of pickups left in game: %i"), Picksups.Num());


    if (Picksups.Num() == 0) {
        AChasingPacmanGameMode* PacmanGameMode = GetWorld()->GetAuthGameMode<AChasingPacmanGameMode>();
        if (PacmanGameMode) {
            PacmanGameMode->EndGame();
        }
    }


    if (Pickup) {
        Pickup->Destroy();
    }

    if(!IsPowered)
    UpdateNearestPickUp(CurrentLocation);

}

void APacmanAIController::Powered(AActor* Pickup , FVector CurrentLocation) {
    int removed = Picksups.Remove(Pickup);

    if (Pickup) {
        Pickup->Destroy();
    }

    UpdateNearestActor(CurrentLocation);

}

void APacmanAIController::UpdateNearestActor(FVector CurrentLocation) {
    FVector MoveLocation;
    double distance = TNumericLimits<double>::Max();

    for (AActor* player : Players) {
        if (player) {
            double temp = FVector::Dist(CurrentLocation, player->GetActorLocation());
            if (temp < distance) {
                distance = temp;
                MoveLocation = player->GetActorLocation();
            }
        }
    }

    GetBlackboardComponent()->SetValueAsVector(TEXT("NearestActor"), MoveLocation);
    IsPowered = true;
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsPowered"), IsPowered);
}

void APacmanAIController::UnPowered(FVector CurrentLocation) {
    IsPowered = false;
    GetBlackboardComponent()->SetValueAsBool(TEXT("IsPowered"), IsPowered);
    UpdateNearestPickUp(CurrentLocation);
}

FVector APacmanAIController::NearestActor() {
    MyCharacter = MyCharacter == nullptr ? GetCharacter() : MyCharacter;
    if (!MyCharacter)return FVector(-1 ,-1,-1);
    double distance = TNumericLimits<double>::Max();
    FVector CurrentLocation = GetCharacter()->GetActorLocation();
    FVector MoveLocation;
    for (AActor* player : Players) {
        if (player) {
            double temp = FVector::Dist(CurrentLocation, player->GetActorLocation());
            if (temp < distance) {
                distance = temp;
                MoveLocation = player->GetActorLocation();
            }
        }
    }
    return MoveLocation;
}