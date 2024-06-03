// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDataAsset.h"

void UWeaponDataAsset::Activate(const APawn* Pawn)
{
	Super::Activate(Pawn);

	if (APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController()))
	{
		// Get the players cameras location and orientation
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// Start the line trace at the center of the camera and shoot straight at the desired range
		FVector Start = CameraLocation;
		FVector End = CameraLocation + (CameraRotation.Vector() * Range);

		// Out hit result
		FHitResult HitResult;

		// Collision conditions and parameters
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(Pawn);

		// Do the line trace
		bool bHit = Pawn->GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Pawn,
			CollisionParams
		);

		// Run if we hit a pawn
		if (bHit && Cast<APawn>(HitResult.GetActor()))
		{
			// Draw line to the hit location and sphere at the hit location
			DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(), HitResult.Location, FColor::Green, false, 2.f, 0, 5.f);
			DrawDebugSphere(Pawn->GetWorld(), HitResult.Location, 10.0f, 12, FColor::Red, false, 2.0f);
		}
	}
}
