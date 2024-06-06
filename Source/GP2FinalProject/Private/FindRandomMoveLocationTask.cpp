// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomMoveLocationTask.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"


UFindRandomMoveLocationTask::UFindRandomMoveLocationTask()
{
	LocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UFindRandomMoveLocationTask, LocationKey));
}

EBTNodeResult::Type UFindRandomMoveLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the AI controller using this behavior tree
	if (AAIController* AIController = OwnerComp.GetAIOwner())
	{
		// Get the pawn off the controller
		if (APawn* Pawn = AIController->GetPawn())
		{
			// Get the navigation system
			if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(Pawn->GetWorld()))
			{
				FVector Origin = Pawn->GetActorLocation();

				FNavLocation Result;

				// Find a random point the pawn could navigate to
				bool bSuccess = NavSystem->GetRandomPointInNavigableRadius(Origin, Range, Result);

				if (bSuccess)
				{
					if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
					{
						// Set the input blackboard vector value to the new point we found
						BlackboardComp->SetValueAsVector(LocationKey.SelectedKeyName, Result.Location);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}
