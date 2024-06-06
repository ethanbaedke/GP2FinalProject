// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "UObject/ConstructorHelpers.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AEnemyAIController::AEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> Target(TEXT("/Game/ArtificialIntelligence/BT_Enemy"));
	if (Target.Succeeded())
	{
		BehaviorTree = Target.Object;
	}

	// Set up sight configuration for perception component

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Config - Sight"));
	SightConfig->SightRadius = 2000.f;
	SightConfig->LoseSightRadius = 2000.f;
	SightConfig->SetMaxAge(0.f);
	SightConfig->PeripheralVisionAngleDegrees = 90.f;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// Setup perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));
	PerceptionComponent->ConfigureSense(*SightConfig);
	SetPerceptionComponent(*PerceptionComponent);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::PerceptionUpdatedCallback);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}
}

void AEnemyAIController::PerceptionUpdatedCallback(const TArray<AActor*>& UpdatedActors)
{
	// Get the pawn we are currently possessing, if we are possessing one
	if (APawn* PossessedPawn = GetPawn())
	{
		FVector CurrentLocation = PossessedPawn->GetActorLocation();

		// Find the closest perceived actor to the enemy
		AActor* ClosestActor = UpdatedActors[0];
		float ClosestDistance = (ClosestActor->GetActorLocation() - CurrentLocation).SquaredLength();
		for (int i = 1; i < UpdatedActors.Num(); i++)
		{
			FVector ActorLocation = UpdatedActors[i]->GetActorLocation();
			float ActorDistance = (ActorLocation - CurrentLocation).SquaredLength();

			if (ActorDistance < ClosestDistance)
			{
				ClosestActor = UpdatedActors[i];
				ClosestDistance = ActorDistance;
			}
		}

		Blackboard->SetValueAsObject(FName("TargetActor"), ClosestActor);
	}
}
