// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

#include "EnemyCharacter.h"

#include "Components/ArrowComponent.h"

AEnemySpawn::AEnemySpawn()
{
	RootComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Component"));
}

AEnemyCharacter* AEnemySpawn::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;

	return GetWorld()->SpawnActor<AEnemyCharacter>(AEnemyCharacter::StaticClass(), GetActorTransform(), SpawnParams);
}
