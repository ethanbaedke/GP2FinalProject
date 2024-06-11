// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

void UHealthComponent::BeginPlay()
{
	CurrentHealth = StartingHealth;
}

void UHealthComponent::ModifyHealth(int32 Amount, AController* EventInstigator)
{
	// Return if we have no health
	if (CurrentHealth <= 0) return;

	// Modify health by amount
	CurrentHealth += Amount;

	// If we are out of health fire our out of health event
	if (CurrentHealth <= 0 && OnOutOfHealth.IsBound())
	{
		OnOutOfHealth.Execute(EventInstigator);
	}
}

int32 UHealthComponent::GetHealth()
{
	return CurrentHealth;
}

int32 UHealthComponent::GetMaxHealth()
{
	return StartingHealth;
}
