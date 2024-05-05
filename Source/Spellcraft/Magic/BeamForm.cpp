// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamForm.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "TargetAroundEffect.h"
#include "Spellcraft/SpellcraftState.h"
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

	if (BeamEffect->GetFXSystemAsset() == nullptr)
	{
		UNiagaraSystem* NiagaraSystemDefault = Cast<UNiagaraSystem>(StaticLoadObject(UNiagaraSystem::StaticClass(), NULL, TEXT("/Script/Niagara.NiagaraSystem'/Game/Blueprints/Magic/Particle/NS_Laser.NS_Laser'")));
		if (NiagaraSystemDefault != nullptr)
		{
			BeamEffect->SetAsset(NiagaraSystemDefault);
		}
	}

	if (GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), TraceEnd, ECollisionChannel::ECC_Camera, CollisionQueryParams))
	{
		OnHit(HitResult);
	}
	else
	{
		if (BeamEffect->GetFXSystemAsset())
		{
			BeamEffect->SetVectorParameter(FName("BeamEnd"), TraceEnd);
		}
	}
}

void ABeamForm::OnHit(const FHitResult& Hit)
{
	if (BeamEffect->GetFXSystemAsset())
	{
		BeamEffect->SetVectorParameter(FName("BeamEnd"), Hit.Location);
	}

	ASpellcraftState* SpellCraftGameState = Cast<ASpellcraftState>(GetWorld()->GetGameState());
	if (!SpellCraftGameState) {
		return;
	}
	
	FSpellData* TempSpellData = SpellCraftGameState->ReadSpellDataById(SpellData->OnCollide);
	if (!TempSpellData)
	{
		return;
	}

	FVector ImpactPoint = Hit.ImpactPoint;

	HitActor = Hit.GetActor();
	HitActorLocation = ImpactPoint;

	UTargetAroundEffect* MagicEffect = NewObject<UTargetAroundEffect>(GetWorld());
	MagicEffect->Id = SpellData->OnCollide;
	MagicEffect->TargetCenter = HitActorLocation;
	MagicEffect->HitActor = HitActor;

	MagicEffect->Execute(nullptr);
}
