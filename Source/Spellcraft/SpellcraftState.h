// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpellcraftState.generated.h"

struct FSpellData;
/**
 * 
 */
UCLASS()
class SPELLCRAFT_API ASpellcraftState : public AGameState
{
	GENERATED_BODY()
	
public:

	virtual bool ReadSpellDataById(int32 SpellId, FSpellData& SpellData);
};
