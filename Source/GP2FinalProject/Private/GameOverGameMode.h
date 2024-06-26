// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameOverGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AGameOverGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIOverlayWidgetClass;

	TObjectPtr<UUserWidget> UIOverlayWidget;
	
};
