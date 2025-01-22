// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellcraftState.h"
#include "Magic/BaseForm.h"
#include "SpellCraftGameInstance.h"


bool ASpellcraftState::ReadSpellDataById(int32 SpellId, FSpellData& SpellData)
{
	USpellCraftGameInstance* GameInstance = Cast<USpellCraftGameInstance>(GetGameInstance());

	if (!GameInstance->SpellDataTable)
		return false;

	static const FString ContextString(TEXT("My Data Table Context"));
	FString NameString = FString::FromInt(SpellId);
	FName RowName = FName(*NameString);
	FSpellData* SpellDataPtr = GameInstance->SpellDataTable->FindRow<FSpellData>(RowName, ContextString);

	if (!SpellDataPtr)
	{
		return false;
	}

	SpellData = *SpellDataPtr;
	return true;
}
