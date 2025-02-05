#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Idle UMETA(DisplayName = "Idle"),
	ECS_Crouching UMETA(DisplayName = "Crouching"),
	ECS_Sprinting UMETA(DisplayName = "Running")
};