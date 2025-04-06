// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Interfaces/WeaponInterface.h"
#include "Power.generated.h"

/**
 * 
 */
UCLASS()
class FPIDEAS_API APower : public ABaseWeapon, public IWeaponInterface
{
	GENERATED_BODY()

public:
	virtual void AttackTriggered() override;

protected:
	virtual void BeginPlay() override;
};
