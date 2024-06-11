// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

class AEnemyCharacter;

UCLASS()
class AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:

	AEnemyCharacter* SpawnEnemy();

private:

	AEnemySpawn();

};
