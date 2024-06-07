// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"

void AGameGameMode::StartPlay()
{
	Super::StartPlay();

	// Add the second player to the game
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		FString ErrorString;
		ULocalPlayer* LocalPlayer = GameInstance->CreateLocalPlayer(1, ErrorString, true);
	}

	// Subscribe to players dying
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* PlayerController = It->Get();

		if (APawn* Pawn = PlayerController->GetPawn())
		{
			if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Pawn))
			{
				PlayerCharacter->OnPlayerDeath.AddUObject(this, &AGameGameMode::PlayerDeathCallback);
			}
		}
	}

	// Spawn enemies

	FActorSpawnParameters SpawnParams;

	for (int32 i = -1000; i <= 1000; i += 1000)
	{
		AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(AEnemyCharacter::StaticClass(), FVector(0, i, 100), FRotator::ZeroRotator, SpawnParams);

		Enemy->OnEnemyDeath.AddUObject(this, &AGameGameMode::EnemyDeathCallback);
	}
}

void AGameGameMode::PlayerDeathCallback(APlayerController* PlayerController)
{
	if (APawn* Pawn = PlayerController->GetPawn())
	{
		Pawn->Destroy();
	}

	AActor* PlayerStart = FindPlayerStart(PlayerController);

	FVector SpawnLocation = PlayerStart->GetActorLocation();
	FRotator SpawnRotation = PlayerStart->GetActorRotation();

	FActorSpawnParameters SpawnParams;

	APlayerCharacter* NewPlayerCharacter = GetWorld()->SpawnActor<APlayerCharacter>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (NewPlayerCharacter)
	{
		NewPlayerCharacter->OnPlayerDeath.AddUObject(this, &AGameGameMode::PlayerDeathCallback);
		PlayerController->Possess(NewPlayerCharacter);
	}
}

void AGameGameMode::EnemyDeathCallback(AEnemyCharacter* EnemyCharacter)
{
	EnemyCharacter->Destroy();
}
