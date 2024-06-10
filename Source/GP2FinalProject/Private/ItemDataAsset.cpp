// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataAsset.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void UItemDataAsset::Activate(APawn* Pawn)
{
	if (ActivationSound)
	{
		UGameplayStatics::PlaySoundAtLocation(Pawn->GetWorld(), ActivationSound, Pawn->GetActorLocation());
	}
}
