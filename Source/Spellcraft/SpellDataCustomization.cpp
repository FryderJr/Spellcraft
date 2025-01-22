// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellDataCustomization.h"
#include "PropertyHandle.h"
#include "IDetailChildrenBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboBox.h"
#include "Magic/MagicBlueprintFunctionLibrary.h"


TSharedRef<IPropertyTypeCustomization> FSpellDataCustomization::MakeInstance()
{
	return TSharedRef<IPropertyTypeCustomization>(new FSpellDataCustomization());
}

void FSpellDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    /*HeaderRow
        .NameContent()
        [
            PropertyHandle->CreatePropertyNameWidget()
        ]
        .ValueContent()
        .MinDesiredWidth(500.0f)
        [
            SNew(SButton)
                .Text(FText::FromString("Add Parameter"))
                .OnClicked(this, &FSpellDataCustomization::OnAddParameterClicked, PropertyHandle)
        ];*/
}

void FSpellDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
    ChildBuilder.AddCustomRow(FText::FromString(TEXT("Add New Property")))
        .ValueContent()
        [
            SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                .Padding(5.0f)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Add Integer Property"))
                        .OnClicked(this, &FSpellDataCustomization::OnAddIntegerPropertyClicked, PropertyHandle)
                ]
                + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                .Padding(5.0f)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Add Float Property"))
                        .OnClicked(this, &FSpellDataCustomization::OnAddFloatPropertyClicked, PropertyHandle)
                ]
        ];
}

FReply FSpellDataCustomization::OnAddParameterClicked(TSharedRef<IPropertyHandle> PropertyHandle)
{
	return FReply::Handled();
}

FReply FSpellDataCustomization::OnAddIntegerPropertyClicked(TSharedRef<IPropertyHandle> PropertyHandle)
{
    /*UE_LOG(LogTemp, Warning, TEXT("Attempting to add integer property"));

    // Create a new FSpellParamInt instance
    FSpellParamInt NewParam;
    NewParam.Value = 20;

    // Get the address of the property
    TArray<void*> RawData;
    PropertyHandle->AccessRawData(RawData);

    void* RawValue;
    PropertyHandle->GetValueData(RawValue);
    UE_LOG(LogTemp, Warning, TEXT("Property class name is %s"), *PropertyHandle->GetProperty()->GetClass()->GetName());

    UE_LOG(LogTemp, Error, TEXT("Raw value address is %x"), RawValue);

    FStructProperty* StructProperty = CastField<FStructProperty>(PropertyHandle->GetProperty());
    if (StructProperty)
    {
        /*FMemory::Memzero(RawValue, StructProperty->Struct->GetStructureSize());
        FMemory::Memcpy(RawValue, (void*)&NewParam, sizeof(FSpellParamInt));

        // Verify the copy operation
        FSpellParamInt* VerifyParam = reinterpret_cast<FSpellParamInt*>(RawValue);
        if (VerifyParam->Value == 20)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully copied new integer property value"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to copy new integer property value"));
        }

        FString SerializedStruct = FString::Printf(TEXT("(Value=%d)"), NewParam.Value);

        if (PropertyHandle->SetValueFromFormattedString(SerializedStruct, EPropertyValueSetFlags::NotTransactable) == FPropertyAccess::Success)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully set integer property value"));
        }

        // Manually copy the data from NewParam to CurrentData
        //StructProperty->Struct->CopyScriptStruct(CurrentData, &NewParam);

        

        // Notify the property change
        PropertyHandle->NotifyPreChange();
        PropertyHandle->NotifyPostChange(EPropertyChangeType::ValueSet);

        // Serialize the new structure to a string
        StructProperty->Struct->ExportText(SerializedStruct, RawValue, RawValue, nullptr, PPF_None, nullptr);
        UE_LOG(LogTemp, Warning, TEXT("New Integer Property Structure: %s"), *SerializedStruct);

        // Refresh the editor UI
        if (FPropertyEditorModule* PropertyModule = FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Notifying property editor module changed"));
            PropertyModule->NotifyCustomizationModuleChanged();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get PropertyEditor module"));
        }

        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Integer Property Added"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to cast to FStructProperty"));
        return FReply::Unhandled();
    }*/

    return FReply::Handled();
}

FReply FSpellDataCustomization::OnAddFloatPropertyClicked(TSharedRef<IPropertyHandle> PropertyHandle)
{
    return FReply::Handled();
}
