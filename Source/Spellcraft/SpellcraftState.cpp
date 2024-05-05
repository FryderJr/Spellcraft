// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellcraftState.h"
#include "Magic/BaseForm.h"
#include "SpellCraftGameInstance.h"

FSpellData* ASpellcraftState::ReadSpellDataById(int32 SpellId)
{
	USpellCraftGameInstance* GameInstance = Cast<USpellCraftGameInstance>(GetGameInstance());
	FSpellData* TempSpellData = nullptr;
	if (GameInstance->SpellDataTable)
	{
		static const FString ContextString(TEXT("My Data Table Context"));
		FString NameString = FString::FromInt(SpellId);
		FName RowName = FName(*NameString);

		TempSpellData = GameInstance->SpellDataTable->FindRow<FSpellData>(RowName, ContextString);
	}
	return TempSpellData;
}
