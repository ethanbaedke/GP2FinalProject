// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Setup spring arm component that will hold items
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	// Setup first person camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Make sure the player using this mesh cannot see it
	GetMesh()->SetOwnerNoSee(true);

	// Get the controller of this player
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get the local player off the controller
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			// Get the enhanced input subsystem off the local player
			if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				// Add our player input mapping context to our players input subsystem so it can be used
				if (PlayerInputMapping)
				{
					InputSubsystem->AddMappingContext(PlayerInputMapping, 0);
				}
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind the input actions to their respective functions
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input)
	{
		if (JumpAction)
		{
			Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::JumpCallback);
		}
		if (LookAction)
		{
			Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookCallback);
		}
		if (MoveAction)
		{
			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCallback);
		}
	}
}

void APlayerCharacter::JumpCallback()
{
	Jump();
}

void APlayerCharacter::LookCallback(const FInputActionValue& Value)
{
	// Get the input vector
	FVector2D InputValue = Value.Get<FVector2D>();

	// Set the players control rotation to match the input
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(InputValue.Y);
}

void APlayerCharacter::MoveCallback(const FInputActionValue& Value)
{
	// Get the input vector
	FVector2D InputValue = Value.Get<FVector2D>();

	// Get the players control rotation
	FRotator ControlRotation = Controller->GetControlRotation();

	// Zero out the pitch since we dont want the player looking up or down to affect the movement direction
	ControlRotation.Pitch = 0;

	// Get the forward and right axis relative to where the character is looking
	FVector Forward = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);
	FVector Right = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);

	// Add movement in the directions of our found axis with the amounts of our input
	AddMovementInput(Forward, InputValue.Y);
	AddMovementInput(Right, InputValue.X);
}

