#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Crouching UMETA(DisplayName = "Crouching"),
	ECS_Sprinting UMETA(DisplayName = "Running")
};

UENUM(BlueprintType)
enum class EWeaponHeld : uint8
{
	EWH_None UMETA(DisplayName = "None"),
	EWH_HitScan UMETA(DisplayName = "HitScan"),
	EWH_Projectile UMETA(DisplayName = "Projectile"),
	EWH_Melee UMETA(DisplayName = "Melee")
};