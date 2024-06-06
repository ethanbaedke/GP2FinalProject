// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "EnemyAIController.h"

#include "HealthComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	// Set the AI class that this character should use to be the Enemy AI Controller class
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// Setup health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnOutOfHealth.BindUObject(this, &AEnemyCharacter::OutOfHealthCallback);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TotalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ModifyHealth(-TotalDamage);

	return TotalDamage;
}

void AEnemyCharacter::OutOfHealthCallback()
{
	if (OnEnemyDeath.IsBound())
	{
		OnEnemyDeath.Broadcast(this);
	}
}
