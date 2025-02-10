// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/HitScan.h"

void AHitScan::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("HitScan"));
}