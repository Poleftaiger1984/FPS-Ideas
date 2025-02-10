// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Power.generated.h"

/**
 * 
 */
UCLASS()
class FPIDEAS_API APower : public ABaseWeapon
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
