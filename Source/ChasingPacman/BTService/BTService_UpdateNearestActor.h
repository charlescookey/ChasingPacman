// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateNearestActor.generated.h"

/**
 * 
 */
UCLASS()
class CHASINGPACMAN_API UBTService_UpdateNearestActor : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTService_UpdateNearestActor();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY()
	class APacmanAIController* MyController;
};
