// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBullet.generated.h"

class UNiagaraComponent;
class UProjectileMovementComponent;

UCLASS()
class FPIDEAS_API AProjectileBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBullet();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> ProjectileEffect;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

};
