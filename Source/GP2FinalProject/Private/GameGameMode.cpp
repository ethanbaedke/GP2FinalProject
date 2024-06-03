// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"

void AGameGameMode::StartPlay()
{
	Super::StartPlay();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		FString ErrorString;
		ULocalPlayer* LocalPlayer = GameInstance->CreateLocalPlayer(1, ErrorString, true);
	}

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