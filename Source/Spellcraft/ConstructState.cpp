// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructState.h"
#include "Magic/BaseForm.h"
#include "Magic/BeamForm.h"
#include "Magic/ProjectileForm.h"

AConstructState::AConstructState()
{
	/*FSpellData* InitialSpell = new FSpellData();
	InitialSpell->bInitial = true;
	InitialSpell->Form = EFormType::Projectile;
	Spells.Add(InitialSpell);*/
}

void AConstructState::SpawnSpell()
{
	if (SpellSpawnPoint)
	{
		for (size_t i = 0; i < Spells.Num(); i++)
		{
			if (Spells[i].bInitial)
			{
				ABaseForm* SpellSpawned;
				switch (Spells[i].Form)
				{
				case EFormType::Projectile: SpellSpawned = GetWorld()->SpawnActor<AProjectileForm>(SpellSpawnPoint->GetActorLocation() + SpellSpawnPoint->GetActorForwardVector() * 100.0f, SpellSpawnPoint->GetActorRotation()); break;
				case EFormType::Beam: SpellSpawned = GetWorld()->SpawnActor<ABeamForm>(SpellSpawnPoint->GetActorLocation() + SpellSpawnPoint->GetActorForwardVector() * 100.0f, SpellSpawnPoint->GetActorRotation()); break;
				default:
					break;
				}
			}
		}
	}
}

FSpellData* AConstructState::ReadSpellDataById(int32 SpellId)
{
	if (SpellId >= Spells.Num() || SpellId < 0)
	{
		return nullptr;
	}
	return &Spells[SpellId];
}
