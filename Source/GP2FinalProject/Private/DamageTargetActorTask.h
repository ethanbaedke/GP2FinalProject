// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DamageTargetActorTask.generated.h"

/**
 * 
 */
UCLASS()
class UDamageTargetActorTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	UDamageTargetActorTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ActorKey;

	UPROPERTY(EditAnywhere)
	float Damage;

};
