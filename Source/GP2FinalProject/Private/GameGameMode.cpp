// Fill out your copyright notice in the Description page of Project Settings.


#include "GameGameMode.h"

#include "Kismet/GameplayStatics.h"

void AGameGameMode::StartPlay()
{
	Super::StartPlay();

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		FString ErrorString;
		ULocalPlayer* LocalPlayer = GameInstance->CreateLocalPlayer(1, ErrorString, true);
	}
}
