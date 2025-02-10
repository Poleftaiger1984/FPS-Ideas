// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectile.h"

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Projectile"));
}
