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

class UCharacterMovementComponent;

class UHealthComponent;

class UAIPerceptionStimuliSourceComponent;

class USoundCue;

// Called when the player runs out of health
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDeath, APlayerController*)

UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	FOnPlayerDeath OnPlayerDeath;

private:

	APlayerCharacter();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Item
	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemDataAsset> ItemDataAsset;

	float UseItemTimer = 0.f;


	// Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PrivateItemMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> PublicItemMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAIPerceptionStimuliSourceComponent> PerceptionSourceComponent;


	// Input
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


	// Movement
	TObjectPtr<UCharacterMovementComponent> MovementComponent;


	// Health
	UPROPERTY(EditAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	void OutOfHealthCallback();

	// Sound
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> FootstepSound;

	float FootstepTimer = 0.f;

};
