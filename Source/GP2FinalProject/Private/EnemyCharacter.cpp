// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "EnemyAIController.h"

#include "Components/CapsuleComponent.h"

#include "HealthComponent.h"


AEnemyCharacter::AEnemyCharacter()
{
	// Set the AI class that this character should use to be the Enemy AI Controller class
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	// Setup the mesh
	if (USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>())
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> Finder(TEXT("/Game/Imported/Kraken/Meshes/KRAKEN"));
		if (Finder.Succeeded())
		{
			MeshComp->SetSkeletalMesh(Finder.Object);

			FTransform Transform = FTransform(FRotator(0, -90, 0), FVector(0, 0, -50), FVector(.1, .1, .1));
			MeshComp->SetRelativeTransform(Transform);
		}
	}

	// Setup collider
	if (UCapsuleComponent* CapsuleComp = FindComponentByClass<UCapsuleComponent>())
	{
		CapsuleComp->SetCapsuleSize(50.f, 50.f);
	}

	// Setup health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent->OnOutOfHealth.BindUObject(this, &AEnemyCharacter::OutOfHealthCallback);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float TotalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HealthComponent->ModifyHealth(-TotalDamage);

	return TotalDamage;
}

void AEnemyCharacter::OutOfHealthCallback()
{
	if (OnEnemyDeath.IsBound())
	{
		OnEnemyDeath.Broadcast(this);
	}

	Destroy();
}
