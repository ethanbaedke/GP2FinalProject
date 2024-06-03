// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AGameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:

	virtual void StartPlay() override;

};
