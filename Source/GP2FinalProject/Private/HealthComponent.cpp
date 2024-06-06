// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

void UHealthComponent::ModifyHealth(int32 Amount)
{
	// Return if we have no health
	if (Health <= 0) return;

	// Modify health by amount
	Health += Amount;

	// If we are out of health fire our out of health event
	if (Health <= 0 && OnOutOfHealth.IsBound())
	{
		OnOutOfHealth.Execute();
	}
}
