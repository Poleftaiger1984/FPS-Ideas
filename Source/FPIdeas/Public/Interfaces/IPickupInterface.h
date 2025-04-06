// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Characters/CharacterStates.h"
#include "IPickupInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPickupInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FPIDEAS_API IIPickupInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetWeaponLookedAt(TObjectPtr<class ABaseWeapon> Weapon);
};
