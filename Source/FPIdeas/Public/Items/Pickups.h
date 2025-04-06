// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "Characters/CharacterStates.h"
#include "Pickups.generated.h"

/**
 * 
 */
UCLASS()
class FPIDEAS_API APickups : public ABaseItem
{
	GENERATED_BODY()
	
public:
	FORCEINLINE EBoostType GetBoostType() const { return BoostType; }
	FORCEINLINE int32 GetBoostAmount() const { return BoostAmount; }

	virtual void InteractAction() override;

private:

	UPROPERTY(EditAnywhere, Category = "Pickup Options")
	EBoostType BoostType = EBoostType::EBT_Default;

	UPROPERTY(EditAnywhere, Category = "Pickup Options")
	int32 BoostAmount{ 0 };
};
