// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items/BaseItem.h"
#include "BaseWeapon.generated.h"

class USphereComponent;
class UAnimMontage;

UCLASS()
class FPIDEAS_API ABaseWeapon : public ABaseItem
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();
	virtual void Tick(float DeltaTime) override;
	void Equip(TObjectPtr<USceneComponent> ComponentToEquipTo, FName RightHandSocket, TObjectPtr<AActor> NewOwner, TObjectPtr<APawn> NewInstigator);
	void Unequip();
	void AttackMeshToSocket(TObjectPtr<USceneComponent> MeshToAttachTo, const FName EquipSocket);


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Montages")
	TObjectPtr<UAnimMontage> AttackMontage;

public:

};
