// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellReferenceWidget.generated.h"

class UTextBlock;
class UButton;
enum ESpellProperty : uint8;
/**
 * 
 */
UCLASS()
class SPELLCRAFT_API USpellReferenceWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = Spellcraft)
	void InitializeSpellProperty(ESpellProperty InSpellProperty, int32 InSpellId);

	FString GetSpellPropertyName(ESpellProperty Property) const;

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SpellNameTextbox;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DeleteButton;

	TEnumAsByte<ESpellProperty> SpellProperty;

	int32 SpellId = -1;
	
	void UpdateUI();
};
