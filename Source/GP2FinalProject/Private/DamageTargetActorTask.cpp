// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTargetActorTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

UDamageTargetActorTask::UDamageTargetActorTask()
{
	ActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UDamageTargetActorTask, ActorKey), AActor::StaticClass());
}

EBTNodeResult::Type UDamageTargetActorTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
						UGameplayStatics::ApplyDamage(TargetActor, Damage, AIController, Pawn, nullptr);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
