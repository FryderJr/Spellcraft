// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructState.h"
#include "Magic/BaseForm.h"
#include "Magic/BeamForm.h"
#include "Magic/ProjectileForm.h"
#include "Weapon.h"
#include "Magic/MagicBlueprintFunctionLibrary.h"

AConstructState::AConstructState()
{
	SpellPropertiesByForm = UMagicBlueprintFunctionLibrary::GetPropetyByForm();
	SpellPropertyDataType = UMagicBlueprintFunctionLibrary::GetSpellPropertyMap();
}

void AConstructState::SpawnSpell()
{
	if (SpellSpawnPoint)
	{
		for (size_t i = 0; i < Spells.Num(); i++)
		{
			if (Spells[i].bInitial)
			{
				AActor* SpellSpawned;
				switch (Spells[i].Form)
				{
				case EFormType::Projectile: SpellSpawned = GetWorld()->SpawnActor<AProjectileForm>(SpellSpawnPoint->GetActorLocation() + SpellSpawnPoint->GetActorForwardVector() * 100.0f, SpellSpawnPoint->GetActorRotation()); break;
				case EFormType::Beam: SpellSpawned = GetWorld()->SpawnActor<AWeapon>(SpellSpawnPoint->GetActorLocation() + SpellSpawnPoint->GetActorForwardVector() * 100.0f, SpellSpawnPoint->GetActorRotation()); SpellSpawned->SetActorHiddenInGame(true); break;
				default:
					break;
				}
			}
		}
	}
}

bool AConstructState::ReadSpellDataById(int32 SpellId, FSpellData& SpellData)
{
	if (SpellId >= Spells.Num() || SpellId < 0)
	{
		return false;
	}
	SpellData = Spells[SpellId];
	return true;
}

int32 AConstructState::AddSpellData()
{
	FSpellData NewSpellData;
	NewSpellData.bInitial = false;
	NewSpellData.Form = EFormType::Projectile;

	if (Spells.IsEmpty())
	{
		NewSpellData.bInitial = true;
	}

	return Spells.Add(NewSpellData);
}

TArray<TEnumAsByte<ESpellProperty>> AConstructState::GetSpellPropertiesByForm(EFormType Form)
{
	return SpellPropertiesByForm[Form];
}

ESpellDataType AConstructState::GetDataTypeBySpellProperty(ESpellProperty SpellProperty)
{
	return SpellPropertyDataType[SpellProperty];
}
