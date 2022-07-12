// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasingPacmanGameMode.h"
#include "ChasingPacman/Character/BaseCharacter.h"
#include "ChasingPacman/PlayerController/ChasingPacmanPlayerController.h"

#include "ChasingPacman/PlayerController/PacmanAIController.h"
#include "ChasingPacman/Character/PacmanCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"


void AChasingPacmanGameMode::PlayerEliminated(ABaseCharacter* ElimmedCharacter, AChasingPacmanPlayerController* VictimController, AController* AttackerController)
{
	UE_LOG(LogTemp, Error, TEXT("Player eliminated called"));
	if (AttackerController == nullptr)return;// || AttackerController->PlayerState == nullptr) return;
	if (VictimController == nullptr)return;// || VictimController->PlayerState == nullptr) return;
	
	//ABlasterPlayerState* AttackerPlayerState = AttackerController ? Cast<ABlasterPlayerState>(AttackerController->PlayerState) : nullptr;
	//ABlasterPlayerState* VictimPlayerState = VictimController ? Cast<ABlasterPlayerState>(VictimController->PlayerState) : nullptr;

	/*if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}*/
	UE_LOG(LogTemp, Error, TEXT("acttack or victim null"));

	if (ElimmedCharacter)
	{
		ElimmedCharacter->Eliminated();
	}
}
void AChasingPacmanGameMode::RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;

		//we need to make different player starts
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		UE_LOG(LogTemp, Warning, TEXT("Number of Player Start: %i"), PlayerStarts.Num());
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}

void AChasingPacmanGameMode::RespawnPacman(APacmanCharacter* DeadPacman){
	if (DeadPacman) {
		DeadPacman->Destroy();
	}

	if (PacmanLives > 0) {
		UWorld* World = GetWorld();
		if (World)
		{
			TArray<AActor*> PlayerStarts;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
			int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
			World->SpawnActor<APacmanCharacter>(
				PacmanClass,
				PlayerStarts[Selection]->GetActorLocation(),
				PlayerStarts[Selection]->GetActorRotation()
				);
		}
	}
}

void AChasingPacmanGameMode::PacmanDied(APacmanCharacter* ElimmedPacman, APacmanAIController* PacmanController, AChasingPacmanPlayerController* AttackerController) {
	if (PacmanController == nullptr)return;
	if (AttackerController == nullptr)return;

	if (ElimmedPacman) {
		ElimmedPacman->Died();
	}

	PacmanLives--;

	UWorld* World = GetWorld();
	if (World) {
		AGameStateBase* MyGameState = World->GetGameState();
		if (MyGameState) {

			for (TObjectPtr< APlayerState> PlayerState : MyGameState->PlayerArray) {
				AChasingPacmanPlayerController* PlayerController = Cast<AChasingPacmanPlayerController>(PlayerState->GetOwner());

				if (PlayerController) {
					UE_LOG(LogTemp, Warning, TEXT("Pacman Lives Left %i"), PacmanLives);
					PlayerController->SetPacmanLives(PacmanLives);
				}
			}
		}
	}

}

void AChasingPacmanGameMode::EndGame() {
	int32 count = 0;
	UWorld* World = GetWorld();
	/*if (World) {
		UE_LOG(LogTemp, Warning, TEXT("World active"));
		for (FConstPlayerControllerIterator Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator) {
			UE_LOG(LogTemp, Warning, TEXT("PlayerController exist"));
			AChasingPacmanPlayerController* PlayerController = Cast< AChasingPacmanPlayerController>(*Iterator);
			count++;
			if (PlayerController) {
				UE_LOG(LogTemp, Warning, TEXT("cast worked and called end cgame on conteoller %s") , *PlayerController->GetName());
				PlayerController->SetEndGame();


				ACharacter* test = PlayerController->GetCharacter();
				if (test) {
					UE_LOG(LogTemp, Warning, TEXT("It's actor Name is %s"), *test->GetName());
				}
			}
		}
	}
	UE_LOG(LogTemp, Error, TEXT("These many plyer controller: %i") , count);*/


	if (World) {
		AGameStateBase* MyGameState = World->GetGameState();
		if (MyGameState) {

			for (TObjectPtr< APlayerState> PlayerState : MyGameState->PlayerArray) {
				AChasingPacmanPlayerController* PlayerController = Cast<AChasingPacmanPlayerController>(PlayerState->GetOwner());

				if (PlayerController) {
					PlayerController->SetEndGame();
				}
			}
		}
	}

}