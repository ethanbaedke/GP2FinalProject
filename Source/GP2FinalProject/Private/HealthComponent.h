// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// Declare a delegate for when health drops to or below zero
// This is singlecast since the only thing that should listen for this is the actor this component is on
DECLARE_DELEGATE(FOnOutOfHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	// Adds the input amount to the current health
	// Use negative numbers for dealing damage and positive numbers for adding health
	void ModifyHealth(int32 Amount);

	FOnOutOfHealth OnOutOfHealth;

private:

	UPROPERTY(EditAnywhere)
	int32 Health = 100;
		
};
