// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateNearestActor.h"
#include "ChasingPacman/PlayerController/PacmanAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateNearestActor::UBTService_UpdateNearestActor()
{
    NodeName = TEXT("Update Nearest Actor");
}

void UBTService_UpdateNearestActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AAIController* TempController = OwnerComp.GetAIOwner();

    if (TempController) {
        MyController = MyController == nullptr ? Cast<APacmanAIController>(TempController) : MyController;
        if (MyController) {
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), MyController->NearestActor());
        }
    }
}
