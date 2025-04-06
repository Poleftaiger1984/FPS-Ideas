// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Misc/ProjectileBullet.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBullet::AProjectileBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Effect"));
	ProjectileEffect->SetupAttachment(ProjectileMesh);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 800.f;
	ProjectileMovement->MaxSpeed = 8500.f;
}

void AProjectileBullet::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

