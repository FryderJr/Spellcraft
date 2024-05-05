// Fill out your copyright notice in the Description page of Project Settings.


#include "ConstructGameMode.h"
#include "Spellcraft/SpellcraftState.h"

AConstructGameMode::AConstructGameMode()
{
	GameStateClass = ASpellcraftState::StaticClass();
}
