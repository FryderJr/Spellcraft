// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellProperties.h"
#include "Components/ScrollBox.h"
#include "SpellReferenceWidget.h"
#include "Spellcraft/ConstructState.h"
#include "Spellcraft/Magic/SpellcraftNode.h"
#include "Spellcraft/Magic/MagicBlueprintFunctionLibrary.h"

void USpellProperties::NativeConstruct()
{
	
}

void USpellProperties::Redraw(ASpellcraftNode* Node)
{
	// Clear all properties before redraw
	DeleteProperties();

	if (!Node)
	{
		return;
	}

	SpellcraftNode = Node;

	AConstructState* GameState = Cast<AConstructState>(GetWorld()->GetGameState());

	if (!GameState)
	{
		return;
	}

	FSpellData SpellData;
	if (GameState->ReadSpellDataById(SpellcraftNode->SpellId, SpellData))
	{
		TArray<TEnumAsByte<ESpellProperty>> SpellProperties = GameState->GetSpellPropertiesByForm(SpellData.Form);
		for (ESpellProperty Property : SpellProperties)
		{
			ESpellDataType SpellDataType = GameState->GetDataTypeBySpellProperty(Property);
			USpellReferenceWidget* SpellReferenceWidgetInstance;
			switch (SpellDataType)
			{
			case ESpellDataType::SpellReference:
				SpellReferenceWidgetInstance = CreateWidget<USpellReferenceWidget>(GetWorld(), SpellPropertyWidgetClass);
				SpellReferenceWidgetInstance->InitializeSpellProperty(Property, SpellcraftNode->SpellId);
				PropertyScrollBox->AddChild(SpellReferenceWidgetInstance);
				break;
			default:
				break;
			}
		}
	}
}

void USpellProperties::DeleteProperties()
{
	PropertyScrollBox->ClearChildren();
}
