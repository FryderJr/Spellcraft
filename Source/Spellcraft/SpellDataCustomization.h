// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "IPropertyTypeCustomization.h"
#include "Input/Reply.h"
#include "PropertyHandle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"

/**
 * 
 */
class SPELLCRAFT_API FSpellDataCustomization : public IPropertyTypeCustomization
{
public:
    // Makes an instance of this class
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

    // IPropertyTypeCustomization interface methods
    virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
    virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
    FReply OnAddParameterClicked(TSharedRef<IPropertyHandle> PropertyHandle);

    FReply OnAddIntegerPropertyClicked(TSharedRef<IPropertyHandle> PropertyHandle);

    FReply OnAddFloatPropertyClicked(TSharedRef<IPropertyHandle> PropertyHandle);
};
