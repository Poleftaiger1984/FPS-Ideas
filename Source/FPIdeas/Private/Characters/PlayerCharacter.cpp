// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributesComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/WeaponInterface.h"
#include "Items/BaseItem.h"
#include "Weapons/BaseWeapon.h"
#include "Items/Pickups.h"
#include "Items/SimpleInteractable.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerCharacter::APlayerCharacter()
	: WeaponClass(EWeaponHeld::EWH_None), CharacterState(ECharacterState::ECS_Idle)
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->AirControlBoostMultiplier = 10000.f;
	GetCharacterMovement()->FallingLateralFriction = 1.f;

	//Control rotation of character with the camera
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetupAttachment(GetRootComponent());

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::Crouch);

		//Attacking
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);

		//Power Attack
		EnhancedInputComponent->BindAction(PowerAttackAction, ETriggerEvent::Started, this, &APlayerCharacter::PowerAttack);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::FinishedSprint);

		//Pressing the E Key
		EnhancedInputComponent->BindAction(EKeyPressedAction, ETriggerEvent::Triggered, this, &APlayerCharacter::EKeyPressed);
	}

}

void APlayerCharacter::Jump()
{
	Super::Jump();

	if (bIsMidair && JumpCounter == 1)
	{
		GetCharacterMovement()->Velocity.Z = FMath::Max<FVector::FReal>(GetCharacterMovement()->Velocity.Z, GetCharacterMovement()->JumpZVelocity);
		JumpCounter = 2;
	}
}

void APlayerCharacter::SetWeaponLookedAt(TObjectPtr<ABaseWeapon> Weapon)
{
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractionTrace(InteractionHit);	
	StaminaHandler(DeltaTime);

	if (GetVelocity().Z != 0)
	{
		bIsMidair = true;
	}
	else if (GetVelocity().Z == 0)
	{
		bIsMidair = false;
		JumpCounter = 0;
	}

	if (JumpCounter == 0 && bIsMidair)
	{
		JumpCounter = 1;
	}	
}

void APlayerCharacter::StaminaHandler(float DeltaTime)
{
	if ((GetCharacterMovement()->Velocity.X == 0 || GetCharacterMovement()->Velocity.Y == 0 || GetCharacterMovement()->MaxWalkSpeed == WalkSpeed) && Attributes && Attributes->GetStamina() < Attributes->GetMaxStamina() && CharacterState != ECharacterState::ECS_Sprinting)
	{
		Attributes->RegenStamina(DeltaTime);
	}

	if ((GetCharacterMovement()->Velocity.X != 0 || GetCharacterMovement()->Velocity.Y != 0) && Attributes && Attributes->GetStamina() > Attributes->GetSprintCost() && CharacterState == ECharacterState::ECS_Sprinting)
	{
		Attributes->UseStamina(Attributes->GetSprintCost(), DeltaTime);
	}

	if (Attributes && Attributes->GetStamina() < Attributes->GetSprintCost() && CharacterState == ECharacterState::ECS_Sprinting)
	{
		FinishedSprint();
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FPContext, 0);
		}
	}

	Tags.Add(FName("Player"));
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (GetController())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		const FRotator ControllerRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);
		
		//Storing which direction the camera/controller is facing
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisValue.X);
	AddControllerPitchInput(LookAxisValue.Y);
}

void APlayerCharacter::Crouch()
{
	if (FirstPersonCamera && CharacterState != ECharacterState::ECS_Crouching)
	{
		FirstPersonCamera->AddWorldOffset(FVector(0.f, 0.f, -CrouchHeight));
		CharacterState = ECharacterState::ECS_Crouching;
		GetCharacterMovement()->MaxWalkSpeed = CrouchWalkSpeed;
	}
	else if (FirstPersonCamera && CharacterState == ECharacterState::ECS_Crouching)
	{
		FirstPersonCamera->AddWorldOffset(FVector(0.f, 0.f, CrouchHeight));
		CharacterState = ECharacterState::ECS_Idle;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::Attack()
{
	if (bIsHoldingWeapon && HeldWeapon)
	{
		WeaponInterface->AttackTriggered();
	}
}

void APlayerCharacter::PowerAttack()
{
	if (bHasPowerWeapon && HeldPowerWeapon)
	{

	}
}

void APlayerCharacter::Sprint()
{
	if (!CanSprint()) return;

	CharacterState = ECharacterState::ECS_Sprinting;
	if (Attributes && Attributes->GetStamina() > Attributes->GetSprintCost())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::FinishedSprint()
{
	if (CharacterState == ECharacterState::ECS_Crouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = CrouchWalkSpeed;
	}
	else
	{
		CharacterState = ECharacterState::ECS_Idle;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::EKeyPressed()
{
	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(InteractionHit.GetActor()); //We have an item in view
	if (InteractionInterface) 
	{
		ABaseWeapon* Weapon = Cast<ABaseWeapon>(InteractionHit.GetActor()); //We have a weapon in view
		if (Weapon)
		{
			//WeaponClass = Weapon->Tag
			if (!bIsHoldingWeapon && ComponentToEquipTo && !Weapon->ActorHasTag("Power"))
			{
				bIsHoldingWeapon = true;
				USceneComponent* WeaponRoot = Weapon->GetRootComponent();
				if (WeaponRoot)
				{
					Weapon->Equip(ComponentToEquipTo, FName(WeaponSocket), this, this);
					FTransform WeaponSocketTranform = Weapon->GetItemMesh()->GetSocketTransform(Weapon->GetGripSocket(), RTS_Component);
					
					FTransform OffsetTransform = WeaponSocketTranform.Inverse(); //Inverting because the grip might be at 10,0,0 but relative to the root it's at -10,0,0

					WeaponRoot->SetRelativeTransform(OffsetTransform);
				}
				ActorsToIgnore.Add(Weapon);
				HeldWeapon = Weapon;
				WeaponInterface = Cast<IWeaponInterface>(HeldWeapon);

				if (HeldWeapon->ActorHasTag("HitScan"))
				{
					WeaponClass = EWeaponHeld::EWH_HitScan;
				}
				if (HeldWeapon->ActorHasTag("Projectile"))
				{
					WeaponClass = EWeaponHeld::EWH_Projectile;
				}
				if (HeldWeapon->ActorHasTag("Melee"))
				{
					WeaponClass = EWeaponHeld::EWH_Melee;
				}
			}
			
			if (!bHasPowerWeapon && ComponentToEquipTo && Weapon->ActorHasTag("Power"))
			{
				bHasPowerWeapon = true;
				Weapon->Equip(ComponentToEquipTo, FName(PowerSocket), this, this);
				HeldPowerWeapon = Weapon;
				WeaponInterface = Cast<IWeaponInterface>(HeldWeapon);
				ActorsToIgnore.Add(Weapon);
			}
		}

		APickups* Pickup = Cast<APickups>(InteractionHit.GetActor()); //We have a pickup in view
		if (Pickup)
		{
			Pickup->InteractAction();
			Attributes->AddBoost(Pickup->GetBoostType(), Pickup->GetBoostAmount());
		}

		ASimpleInteractable* SimIntObject = Cast<ASimpleInteractable>(InteractionHit.GetActor()); //We have a simple interactable object in view
		if (SimIntObject)
		{
			SimIntObject->InteractAction();
		}
	}	
}

void APlayerCharacter::InteractionTrace(FHitResult& SphereHitResult)
{
	const FVector Start = FirstPersonCamera->GetComponentLocation();
	const FVector End = Start + (FirstPersonCamera->GetForwardVector() * InteractRange);

	if (UKismetSystemLibrary::SphereTraceSingle(this, Start, End, CrosshairRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, bShowInteractionDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, SphereHitResult, true))
	{
		SetItemLookedAt(InteractionHit);
	}
}

void APlayerCharacter::SetItemLookedAt(FHitResult& LookTraceResult)
{
	TObjectPtr<AActor> HitActor = LookTraceResult.GetActor();
	if (HitActor != LastSeenActor)
	{
		//The looked at object has changed
		LastSeenActor = HitActor;
		ItemInView = nullptr;

		if (HitActor && HitActor->IsA<ABaseItem>())
		{
			ItemInView = Cast<ABaseItem>(HitActor); //Current Item Looked At
			UE_LOG(LogTemp, Warning, TEXT("%s"), *ItemInView.GetName());
		}
	}
	
}

bool APlayerCharacter::CanSprint()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetSprintCost() && CharacterState != ECharacterState::ECS_Sprinting && CharacterState != ECharacterState::ECS_Crouching;
}
