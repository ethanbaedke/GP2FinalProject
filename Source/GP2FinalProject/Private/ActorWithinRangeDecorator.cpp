// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorWithinRangeDecorator.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UActorWithinRangeDecorator::UActorWithinRangeDecorator()
{
	ActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UActorWithinRangeDecorator, ActorKey), AActor::StaticClass());
}

bool UActorWithinRangeDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		if (UObject* ObjectValue = BlackboardComp->GetValueAsObject(ActorKey.SelectedKeyName))
		{
			if (AActor* TargetActor = Cast<AActor>(ObjectValue))
			{
				if (AAIController* AIController = OwnerComp.GetAIOwner())
				{
					if (APawn* Pawn = AIController->GetPawn())
					{
						FVector TargetActorLocation = TargetActor->GetActorLocation();
						FVector PawnLocation = Pawn->GetActorLocation();

						if ((TargetActorLocation - PawnLocation).Length() <= Range)
						{
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}
