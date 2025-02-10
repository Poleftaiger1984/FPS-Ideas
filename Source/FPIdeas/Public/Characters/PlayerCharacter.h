// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interfaces/IPickupInterface.h"
#include "InputActionValue.h"
#include "Characters/CharacterStates.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ABaseWeapon;

UCLASS()
class FPIDEAS_API APlayerCharacter : public ABaseCharacter, public IIPickupInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void SetWeaponLookedAt(ABaseWeapon* Weapon) override;

	virtual void Tick(float DeltaTime) override;

	void StaminaHandler(float DeltaTime);

protected:
	virtual void BeginPlay() override;

	/* Callbacks for input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Crouch();
	void Attack();
	void Sprint();
	void FinishedSprint();
	void EKeyPressed();

	void InteractionTrace(FHitResult& SphereHitResult);

	bool CanSprint();
	

	/* Input Actions */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> FPContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EKeyPressedAction;

	/* Movement Variables */
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchHeight = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1200.f;

	UPROPERTY(VisibleInstanceOnly, Category = "Movement")
	uint8 JumpCounter = 0;

	UPROPERTY(VisibleInstanceOnly)
	bool bIsMidair = false;

	/* Weapon Variables */
	bool bIsHoldingWeapon = false;
	bool bHasPowerWeapon = false;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<ABaseWeapon> WeaponInView;
	FName WeaponClass;

	/* Interaction Variables */
	FHitResult InteractionHit;
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float InteractRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	float CrosshairRadius = 50.f;

	UPROPERTY(EditAnywhere, Category = "Interaction")
	bool bShowInteractionDebug = false;

private:

	/* Character Components */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Montages", meta = (AllowPrivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Idle;

	EWeaponHeld WeaponHeld = EWeaponHeld::EWH_None;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponSocket;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName PowerSocket;

	TObjectPtr<USkeletalMeshComponent> PlayerMesh;

};
