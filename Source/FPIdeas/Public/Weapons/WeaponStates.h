#pragma once

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	EWC_None UMETA(DisplayName = "None"),
	EWC_HitScan UMETA(DisplayName = "HitScan"),
	EWC_Projectile UMETA(DisplayName = "Projectile"),
	EWC_Power UMETA(DisplayName = "Power")
};