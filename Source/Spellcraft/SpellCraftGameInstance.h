// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpellCraftGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API USpellCraftGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	USpellCraftGameInstance();

	UPROPERTY(BlueprintReadOnly, Category = SpellData)
	class UDataTable* SpellDataTable;
	
	UFUNCTION(BlueprintCallable, Category=SpellData)
	void ReadTable();
};
