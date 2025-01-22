// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellcraftState.h"
#include "Magic/MagicBlueprintFunctionLibrary.h"
#include "ConstructState.generated.h"

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API AConstructState : public ASpellcraftState
{
	GENERATED_BODY()

public:

	AConstructState();

	UPROPERTY(EditDefaultsOnly, Category = "Construct|Spell")
	TArray<FSpellData> Spells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construct|Spell")
	AActor* SpellSpawnPoint;

	TArray<TArray<TEnumAsByte<ESpellProperty>>> SpellPropertiesByForm;

	TMap<TEnumAsByte<ESpellProperty>, ESpellDataType> SpellPropertyDataType;

	UFUNCTION(BlueprintCallable, Category = "Construct|Spell")
	void SpawnSpell();

	UFUNCTION(BlueprintCallable, Category = "Construct|Spell")
	virtual bool ReadSpellDataById(int32 SpellId, FSpellData& SpellData) override;

	UFUNCTION(BlueprintCallable, Category = "Construct|Spell")
	int32 AddSpellData();

	UFUNCTION(BlueprintCallable, Category = "Construct|Spell")
	TArray<TEnumAsByte<ESpellProperty>> GetSpellPropertiesByForm(EFormType Form);

	UFUNCTION(BlueprintCallable, Category = "Construct|Spell")
	ESpellDataType GetDataTypeBySpellProperty(ESpellProperty SpellProperty);
};
