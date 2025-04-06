#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Crouching UMETA(DisplayName = "Crouching"),
	ECS_Sprinting UMETA(DisplayName = "Running"),
	ECS_Dashing UMETA(DisplayName = "Dashing")
};

UENUM(BlueprintType)
enum class EWeaponHeld : uint8
{
	EWH_None UMETA(DisplayName = "None"),
	EWH_HitScan UMETA(DisplayName = "HitScan"),
	EWH_Projectile UMETA(DisplayName = "Projectile"),
	EWH_Melee UMETA(DisplayName = "Melee")
};

UENUM(BlueprintType)
enum class EBoostType : uint8
{
	EBT_Default UMETA(DisplayName = "Default"),
	EBT_Health UMETA(DisplayName = "Health"),
	EBT_Stamina UMETA(DisplayName = "Stamina"),
	EBT_Money UMETA(DisplayName = "Money")
};