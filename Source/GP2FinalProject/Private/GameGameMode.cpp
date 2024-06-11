// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "EnemySpawn.h"

#include "Blueprint/UserWidget.h"

#include "EngineUtils.h"

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

	// Get enemy spawns
	for (TActorIterator<AEnemySpawn> It(GetWorld()); It; ++It)
	{
		AEnemySpawn* EnemySpawn = *It;

		if (EnemySpawn)
		{
			EnemySpawns.Add(EnemySpawn);
		}
	}

	// Set timer to spawn an enemy every few seconds
	GetWorld()->GetTimerManager().SetTimer(EnemySpawnTimer, this, &AGameGameMode::SpawnEnemy, 10.f, true, 0.f);

	// Setup UI overlay
	UIOverlayWidget = CreateWidget(GetWorld(), UIOverlayWidgetClass);
	UIOverlayWidget->AddToViewport();
}

void AGameGameMode::SpawnEnemy()
{
	int32 Index = FMath::RandRange(0, EnemySpawns.Num() - 1);
	EnemySpawns[Index]->SpawnEnemy();
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
	EnemiesKilled++;

	UE_LOG(LogTemp, Warning, TEXT("%d"), EnemiesKilled);

	if (EnemiesKilled == 10)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("L_GameOver"));
	}
}
