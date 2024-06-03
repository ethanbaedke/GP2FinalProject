// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataAsset.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UWeaponDataAsset : public UItemDataAsset
{
	GENERATED_BODY()
	
public:

	virtual void Activate(const APawn* Pawn) override;

private:

	UPROPERTY(EditAnywhere)
	int32 Range;

};
