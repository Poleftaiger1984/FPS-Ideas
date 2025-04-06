// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "SimpleInteractable.generated.h"

/**
 * 
 */
UCLASS()
class FPIDEAS_API ASimpleInteractable : public ABaseItem
{
	GENERATED_BODY()

public:
	virtual void InteractAction() override;

protected:
	virtual void BeginPlay() override;
};
