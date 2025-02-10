// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Power.h"

void APower::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Power"));
}