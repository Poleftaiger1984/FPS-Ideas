// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Interfaces/IPickupInterface.h"

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseWeapon::Equip(TObjectPtr<USceneComponent> ComponentToEquipTo, FName EquipSocket, TObjectPtr<AActor> NewOwner, TObjectPtr<APawn> NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttackMeshToSocket(ComponentToEquipTo, EquipSocket);

	//if(NewOwner->ActorHasTag(FName("Player"))) Play Sound;
}

void ABaseWeapon::AttackMeshToSocket(TObjectPtr<USceneComponent> MeshToAttachTo, const FName EquipSocket)
{
	FAttachmentTransformRules SocketRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(MeshToAttachTo, SocketRules, EquipSocket);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
