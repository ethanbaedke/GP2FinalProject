// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

class USoundCue;

UCLASS(Abstract)
class UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	virtual void Activate(APawn* Pawn);

	int32 GetActivationRate() { return ActivationRate; }

private:

	UPROPERTY(EditAnywhere)
	int32 ActivationRate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundCue> ActivationSound;

};
