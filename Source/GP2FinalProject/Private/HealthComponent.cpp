// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

void UHealthComponent::ModifyHealth(int32 Amount)
{
	Health += Amount;

	if (Health <= 0 && OnOutOfHealth.IsBound())
	{
		OnOutOfHealth.Execute();
	}
}
