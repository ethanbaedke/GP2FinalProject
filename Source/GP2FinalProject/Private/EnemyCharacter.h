// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UHealthComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, AEnemyCharacter*)

UCLASS()
class AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	FOnEnemyDeath OnEnemyDeath;

private:

	AEnemyCharacter();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	void OutOfHealthCallback();

};
