// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BaseWeapon.h"
#include "Interfaces/HitInterface.h"

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
	AttachMeshToSocket(ComponentToEquipTo, EquipSocket);
	
	//if(NewOwner->ActorHasTag(FName("Player"))) Play Sound;
}

void ABaseWeapon::AttachMeshToSocket(TObjectPtr<USceneComponent> MeshToAttachTo, const FName EquipSocket)
{
	FAttachmentTransformRules SocketRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(MeshToAttachTo, SocketRules, EquipSocket);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::ExecuteGetHit(FHitResult& TraceResult)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(TraceResult.GetActor());
	if (HitInterface)
	{
		// Execute_GetHit is automatically generated and is the only way to call a BlueprintNativeFunction in C++
		HitInterface->Execute_GetHit(TraceResult.GetActor(), TraceResult.ImpactPoint, GetOwner());
	}
}
