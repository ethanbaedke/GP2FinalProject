// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameGameMode.generated.h"

class AEnemyCharacter;
class AEnemySpawn;

class UUserWidget;

UCLASS()
class AGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	virtual void StartPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIOverlayWidgetClass;

	TObjectPtr<UUserWidget> UIOverlayWidget;

	void SpawnEnemy();

	void PlayerDeathCallback(APlayerController* PlayerController);

	void EnemyDeathCallback(AEnemyCharacter* EnemyCharacter);

	TArray<TObjectPtr<AEnemySpawn>> EnemySpawns;

	FTimerHandle EnemySpawnTimer;

	int32 EnemiesKilled = 0;

};
