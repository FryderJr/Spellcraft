// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellReferenceWidget.h"
#include "UObject/EnumProperty.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USpellReferenceWidget::NativeConstruct()
{

}

void USpellReferenceWidget::InitializeSpellProperty(ESpellProperty InSpellProperty, int32 InSpellId)
{
	SpellProperty = InSpellProperty;
	SpellId = InSpellId;
	UpdateUI();
}

FString USpellReferenceWidget::GetSpellPropertyName(ESpellProperty Property) const
{
	const UEnum* EnumPtr = StaticEnum<ESpellProperty>();
	if (!EnumPtr)
	{
		return TEXT("Unknown");
	}

	return EnumPtr->GetDisplayNameTextByValue(static_cast<int64>(Property)).ToString();
}

void USpellReferenceWidget::UpdateUI()
{
	if (SpellNameTextbox)
	{
		SpellNameTextbox->SetText(FText::FromString(GetSpellPropertyName(SpellProperty)));
	}
	if (SpellId > -1)
	{
		CreateButton->SetVisibility(ESlateVisibility::Visible);
		SpellNameTextbox->SetVisibility(ESlateVisibility::Collapsed);
		DeleteButton->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		CreateButton->SetVisibility(ESlateVisibility::Collapsed);
		SpellNameTextbox->SetVisibility(ESlateVisibility::Visible);
		DeleteButton->SetVisibility(ESlateVisibility::Visible);
	}
}
