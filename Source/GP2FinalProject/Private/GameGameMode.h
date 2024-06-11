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
	
public:

	UFUNCTION(BlueprintCallable)
	int32 GetTargetScore();

private:

	virtual void StartPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIOverlayWidgetClass;

	TObjectPtr<UUserWidget> UIOverlayWidget;

	void PlayerDeathCallback(APlayerController* PlayerController, AController* EventInstigator);

	int32 TargetScore = 30;

	// Enemy stuff
	void SpawnEnemy();

	void EnemyDeathCallback(AEnemyCharacter* EnemyCharacter, AController* EventInstigator);

	TArray<TObjectPtr<AEnemySpawn>> EnemySpawns;

	FTimerHandle EnemySpawnTimer;

};
