// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellcraftState.h"
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
	TArray<FSpellData>  Spells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Construct|Spell")
	AActor* SpellSpawnPoint;

	UFUNCTION(BlueprintCallable, Category = "Construct|Spell")
	void SpawnSpell();

	virtual FSpellData* ReadSpellDataById(int32 SpellId) override;
	
};
