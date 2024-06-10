// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "ItemDataAsset.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "HealthComponent.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

#include "Sound/SoundCue.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Setup spring arm component that will hold items
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	// Setup first person camera
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Setup the item mesh that the local player will see
	PrivateItemMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Private Item Mesh Component"));
	PrivateItemMeshComponent->SetupAttachment(SpringArmComponent);

	// Setup the item mesh that all other players will see
	PublicItemMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Public Item Mesh Component"));
	PublicItemMeshComponent->SetupAttachment(RootComponent);

	// Get movement component
	MovementComponent = FindComponentByClass<UCharacterMovementComponent>();

	// Setup health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

	// Setup AI perception stimuli source component so enemies can see the player
	PerceptionSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AI Perception Stimuli Source Component"));
	PerceptionSourceComponent->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionSourceComponent->RegisterWithPerceptionSystem();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Make sure the player using this mesh cannot see it
	GetMesh()->SetOwnerNoSee(true);

	// Make sure the only the player can see their private item
	PrivateItemMeshComponent->SetOnlyOwnerSee(true);

	// Make sure the player cannot see their public item
	PublicItemMeshComponent->SetOwnerNoSee(true);

	// Subscribe to the delegate for running out of health
	HealthComponent->OnOutOfHealth.BindUObject(this, &APlayerCharacter::OutOfHealthCallback);
}


void APlayerCharacter::Tick(float DeltaTime)
{
	// De-inrement timers
	UseItemTimer -= DeltaTime;
	FootstepTimer -= DeltaTime;
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TotalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ModifyHealth(-TotalDamage);

	return TotalDamage;
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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
		if (UseItemAction)
		{
			Input->BindAction(UseItemAction, ETriggerEvent::Triggered, this, &APlayerCharacter::UseItemCallback);
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

	// Play footstep sound
	if (FootstepSound && MovementComponent)
	{
		if (FootstepTimer <= 0 && MovementComponent->IsMovingOnGround())
		{
			FootstepTimer = FMath::FRandRange(.25f, .33f);
			float Volume = FMath::FRandRange(.25f, .75f);
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FootstepSound, GetActorLocation(), Volume);
		}
	}
}

void APlayerCharacter::UseItemCallback()
{
	if (UseItemTimer <= 0)
	{
		ItemDataAsset->Activate(this);
		UseItemTimer = 1.f / ItemDataAsset->GetActivationRate();
	}
}

void APlayerCharacter::OutOfHealthCallback()
{
	if (OnPlayerDeath.IsBound())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			OnPlayerDeath.Broadcast(PlayerController);
		}
	}
}