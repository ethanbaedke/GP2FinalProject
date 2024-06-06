// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindRandomMoveLocationTask.generated.h"

/**
 * 
 */
UCLASS()
class UFindRandomMoveLocationTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	UFindRandomMoveLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector LocationKey;

	UPROPERTY(EditAnywhere)
	float Range;
};
