// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
	AEnemyAIController();

	virtual void BeginPlay() override;

	TObjectPtr<UBehaviorTree> BehaviorTree;

	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UFUNCTION()
	void PerceptionUpdatedCallback(const TArray<AActor*>& UpdatedActors);

};
