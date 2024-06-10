// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"

#include "Blueprint/UserWidget.h"

void AMenuGameMode::BeginPlay()
{
	// Setup UI overlay
	UIOverlayWidget = CreateWidget(GetWorld(), UIOverlayWidgetClass);
	UIOverlayWidget->AddToViewport();
}
