// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "EnemySpawn.h"

#include "Blueprint/UserWidget.h"

#include "EngineUtils.h"

#include "GameFramework/PlayerState.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

AGameGameMode::AGameGameMode()
{
	// Setup audio component
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;
}

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

	// Start music
	AudioComponent->SetSound(MusicSound);
	AudioComponent->Play();
}

void AGameGameMode::SpawnEnemy()
{
	int32 Index = FMath::RandRange(0, EnemySpawns.Num() - 1);
	AEnemyCharacter* Enem = EnemySpawns[Index]->SpawnEnemy();
	Enem->OnEnemyDeath.AddUObject(this, &AGameGameMode::EnemyDeathCallback);
}

void AGameGameMode::PlayerDeathCallback(APlayerController* PlayerController, AController* EventInstigator)
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

	// If this player was killed by another player, increase that players score
	if (EventInstigator)
	{
		if (APlayerState* PlayerState = EventInstigator->GetPlayerState<APlayerState>())
		{
			PlayerState->Score += 3;
		}
	}
}

int32 AGameGameMode::GetTargetScore()
{
	return TargetScore;
}

void AGameGameMode::EnemyDeathCallback(AEnemyCharacter* EnemyCharacter, AController* EventInstigator)
{
	if (EventInstigator)
	{
		if (APlayerState* PlayerState = EventInstigator->GetPlayerState<APlayerState>())
		{
			PlayerState->Score += 1;
		}
	}
}
