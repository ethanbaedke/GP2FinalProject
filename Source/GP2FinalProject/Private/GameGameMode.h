// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameGameMode.generated.h"

class AEnemyCharacter;

UCLASS()
class AGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	virtual void StartPlay() override;

	void PlayerDeathCallback(APlayerController* PlayerController);

	void EnemyDeathCallback(AEnemyCharacter* EnemyCharacter);

};
