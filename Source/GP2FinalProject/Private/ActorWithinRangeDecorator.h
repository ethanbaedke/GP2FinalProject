// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "ActorWithinRangeDecorator.generated.h"

/**
 * 
 */
UCLASS()
class UActorWithinRangeDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
	UActorWithinRangeDecorator();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ActorKey;

	UPROPERTY(EditAnywhere)
	float Range;

};
