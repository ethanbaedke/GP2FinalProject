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

#include "HealthComponent.h"

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

	// Setup health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
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

	// Create the player overlay UI
	PlayerOverlayWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerOverlayWidgetClass);

	// If the controller is already connected, give that player the UI
	AddPlayerOverlayWidgetToViewport(PlayerOverlayWidget);

	// Subscribe to the delegate for running out of health
	HealthComponent->OnOutOfHealth.BindUObject(this, &APlayerCharacter::OutOfHealthCallback);
}


void APlayerCharacter::Tick(float DeltaTime)
{
	UseItemTimer -= DeltaTime;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Put the UI on the new players screen
	AddPlayerOverlayWidgetToViewport(PlayerOverlayWidget);
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TotalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ModifyHealth(-TotalDamage);

	return TotalDamage;
}

void APlayerCharacter::AddPlayerOverlayWidgetToViewport(UUserWidget* UserWidget)
{
	if (UserWidget)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			int32 ControllerID = UGameplayStatics::GetPlayerControllerID(PlayerController);

			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);

			if (ControllerID == 0)
			{
				PlayerOverlayWidget->SetRenderTranslation(FVector2D(0, -ViewportSize.Y / 2));
				PlayerOverlayWidget->SetRenderScale(FVector2D(.5f, .5f));
			}
			else
			{
				PlayerOverlayWidget->SetRenderTranslation(FVector2D(0, ViewportSize.Y / 2));
				PlayerOverlayWidget->SetRenderScale(FVector2D(.5f, .5f));
			}

			PlayerOverlayWidget->AddToViewport();
		}
	}
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