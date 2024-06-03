// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UItemDataAsset;

class UCameraComponent;
class USpringArmComponent;

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

class UUserWidget;

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemDataAsset> ItemDataAsset;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PrivateItemMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PublicItemMeshComponent;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> UseItemAction;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> PlayerInputMapping;

	void JumpCallback();
	void LookCallback(const FInputActionValue& Value);
	void MoveCallback(const FInputActionValue& Value);
	void UseItemCallback();

	float UseItemTimer = 0.f;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PlayerOverlayWidgetClass;

	TObjectPtr<UUserWidget> PlayerOverlayWidget;

	// Adds the UI Overlay for players to those players viewports, scaled to fit their viewports
	// This currently only supports two players using the vertical splitscreen layout
	void AddPlayerOverlayWidgetToViewport(UUserWidget* UserWidget);

};
