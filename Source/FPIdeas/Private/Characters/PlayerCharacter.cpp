// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AttributesComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "Items/BaseItem.h"
#include "Weapons/BaseWeapon.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->AirControlBoostMultiplier = 10000.f;
	GetCharacterMovement()->FallingLateralFriction = 1.f;

	//Control rotation of character with the camera
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	//Setup collision responses
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	//Components
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 50.f;
	CameraBoom->bUsePawnControlRotation = false;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetupAttachment(CameraBoom);
	
	PlayerMesh = GetMesh();
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);

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

void APlayerCharacter::SetWeaponLookedAt(ABaseWeapon* Weapon)
{
	WeaponInView = Weapon;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractionTrace(InteractionHit);

	if (PlayerMesh && FirstPersonCamera)
	{
		FRotator CameraRotation = FirstPersonCamera->GetComponentRotation();
		PlayerMesh->SetWorldRotation(CameraRotation);
	}
	
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
	if (CameraBoom && CharacterState != ECharacterState::ECS_Crouching)
	{
		CameraBoom->AddWorldOffset(FVector(0.f, 0.f, -CrouchHeight));
		CharacterState = ECharacterState::ECS_Crouching;
		GetCharacterMovement()->MaxWalkSpeed = CrouchWalkSpeed;
	}
	else if (CameraBoom && CharacterState == ECharacterState::ECS_Crouching)
	{
		CameraBoom->AddWorldOffset(FVector(0.f, 0.f, CrouchHeight));
		CharacterState = ECharacterState::ECS_Idle;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void APlayerCharacter::Attack()
{
	if (bIsHoldingWeapon || bHasPowerWeapon)
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
	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(InteractionHit.GetActor());
	if (InteractionInterface) //We have an item in view
	{
		ABaseWeapon* Weapon = Cast<ABaseWeapon>(InteractionHit.GetActor()); //We have a weapon in view
		if (Weapon)
		{
			//WeaponClass = Weapon->Tag
			if (!bIsHoldingWeapon && !Weapon->ActorHasTag("Power"))
			{
				bIsHoldingWeapon = true;
				Weapon->Equip(GetMesh(), FName(WeaponSocket), this, this);
				WeaponInView = nullptr;
				ActorsToIgnore.Add(Weapon);
				HeldWeapon = Weapon;

				if (HeldWeapon->ActorHasTag("HitScan"))
				{
					WeaponHeld = EWeaponHeld::EWH_HitScan;
				}
				if (HeldWeapon->ActorHasTag("Projectile"))
				{
					WeaponHeld = EWeaponHeld::EWH_Projectile;
				}
				if (HeldWeapon->ActorHasTag("Melee"))
				{
					WeaponHeld = EWeaponHeld::EWH_Melee;
				}
			}
			
			if (!bHasPowerWeapon && Weapon->ActorHasTag("Power"))
			{
				bHasPowerWeapon = true;
				Weapon->Equip(GetMesh(), FName(PowerSocket), this, this);
				WeaponInView = nullptr;
				ActorsToIgnore.Add(Weapon);
			}
		}
	}
	
}

void APlayerCharacter::InteractionTrace(FHitResult& SphereHitResult)
{
	const FVector Start = FirstPersonCamera->GetComponentLocation();
	const FVector End = Start + (FirstPersonCamera->GetForwardVector() * InteractRange);

	UKismetSystemLibrary::SphereTraceSingle(this, Start, End, CrosshairRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, bShowInteractionDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, SphereHitResult, true);
}

bool APlayerCharacter::CanSprint()
{
	return Attributes && Attributes->GetStamina() > Attributes->GetSprintCost() && CharacterState != ECharacterState::ECS_Sprinting && CharacterState != ECharacterState::ECS_Crouching;
}
