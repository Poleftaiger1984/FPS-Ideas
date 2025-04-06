// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributesComponent.h"
#include "Characters/CharacterStates.h"

UAttributesComponent::UAttributesComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributesComponent::UseStamina(float StaminaCost, float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina - StaminaCost * DeltaTime, 0.f, MaxStamina);
}

void UAttributesComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UAttributesComponent::AddBoost(EBoostType TypeOfBoost, float BoostAmount)
{
	switch (TypeOfBoost)
	{
		case EBoostType::EBT_Health:
		{
			Health += BoostAmount;
			break;
		}
		case EBoostType::EBT_Stamina:
		{
			Stamina += BoostAmount;
			break;
		}
		case EBoostType::EBT_Money:
		{
			Money += BoostAmount;
			break;
		}
		default:
			break;
	}
}

