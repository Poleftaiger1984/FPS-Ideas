// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "Interfaces/WeaponInterface.h"
#include "HitScan.generated.h"

/**
 * 
 */
UCLASS()
class FPIDEAS_API AHitScan : public ABaseWeapon, public IWeaponInterface
{
	GENERATED_BODY()
	
public:
	void HitScanAttack();
	void BulletLineTrace(FHitResult& LineHit);
	virtual void AttackTriggered() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName MuzzleSocket;

	UPROPERTY(EditAnywhere, Category = "Combat")
	bool bShowBulletTrace;
};
