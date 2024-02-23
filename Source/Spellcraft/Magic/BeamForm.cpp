// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamForm.h"
#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"

ABeamForm::ABeamForm()
{
	PrimaryActorTick.bCanEverTick = false;

	BeamEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("BeamEffect"));
	BeamEffect->SetupAttachment(RootComponent);
}

void ABeamForm::BeginPlay()
{
	Super::BeginPlay();

	FVector TraceEnd = GetActorLocation() + GetActorForwardVector() * 10000;
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(ActorToIgnore);
	//DrawDebugLine(GetWorld(), GetActorLocation(), TraceEnd, FColor::Cyan, false, 10.0f);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), TraceEnd, ECollisionChannel::ECC_Camera, CollisionQueryParams))
	{
		OnHit(HitResult);
	}
	else
	{
		BeamEffect->SetVectorParameter(FName("BeamEnd"), TraceEnd);
	}
}

void ABeamForm::OnHit(const FHitResult& Hit)
{
	BeamEffect->SetVectorParameter(FName("BeamEnd"), Hit.Location);

	FSpellData* TempSpellData = ReadSpellDataById(SpellData->OnCollide);
	if (!TempSpellData)
	{
		return;
	}

	FVector ImpactPoint = Hit.ImpactPoint;

	HitActor = Hit.GetActor();
	HitActorLocation = ImpactPoint;

	StartSpellOverlapCheck(ImpactPoint);
}
