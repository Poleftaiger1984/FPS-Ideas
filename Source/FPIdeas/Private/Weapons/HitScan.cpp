// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/HitScan.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AHitScan::HitScanAttack()
{
	FHitResult BulletResult;
	BulletLineTrace(BulletResult);

	if (BulletResult.GetActor())
	{
		UGameplayStatics::ApplyDamage(BulletResult.GetActor(), Damage, GetInstigator()->GetController(), GetOwner(), UDamageType::StaticClass());
		ExecuteGetHit(BulletResult);
		CreateFields(BulletResult.ImpactPoint);
	}
}

void AHitScan::BulletLineTrace(FHitResult& LineHit)
{
	FVector Start = ItemMesh->GetSocketLocation(MuzzleSocket);
	FVector End = Start + (ItemMesh->GetForwardVector() * AttackRange);
	TArray<AActor*> ActorsToIgnore;

	UKismetSystemLibrary::LineTraceSingle(this, Start, End, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, bShowBulletTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None, LineHit, true);
}

void AHitScan::AttackTriggered()
{
	HitScanAttack();
	UE_LOG(LogTemp, Warning, TEXT("Yay!!!"));
}

void AHitScan::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(FName("HitScan"));
}