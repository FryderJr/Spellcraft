// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCraftGameInstance.h"
#include "Engine/DataTable.h"

USpellCraftGameInstance::USpellCraftGameInstance()
{
	ConstructorHelpers::FObjectFinder<UDataTable> DataTableAsset(TEXT("DataTable'/Game/SpellDataTable.SpellDataTable'"));
	SpellDataTable = DataTableAsset.Object;
}

void USpellCraftGameInstance::ReadTable()
{
	
}
