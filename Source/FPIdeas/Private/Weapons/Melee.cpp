// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Melee.h"

void AMelee::AttackTriggered()
{
}

void AMelee::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("Melee"));
}