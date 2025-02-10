// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAttributesComponent;
class ABaseWeapon;

UCLASS()
class FPIDEAS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributesComponent> Attributes;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABaseWeapon> HeldWeapon; //Reference to currently held weapon

private:
};
