// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "HitScan.generated.h"

/**
 * 
 */
UCLASS()
class FPIDEAS_API AHitScan : public ABaseWeapon
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
};
