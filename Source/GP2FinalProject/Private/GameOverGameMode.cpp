// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverGameMode.h"

#include "Blueprint/UserWidget.h"

void AGameOverGameMode::BeginPlay()
{
	// Setup UI overlay
	UIOverlayWidget = CreateWidget(GetWorld(), UIOverlayWidgetClass);
	UIOverlayWidget->AddToViewport();
}
