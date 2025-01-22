// Copyright Epic Games, Inc. All Rights Reserved.

#include "Spellcraft.h"
#include "Modules/ModuleManager.h"
#include "SpellDataCustomization.h"  

class FSpellcraftGameModule : public FDefaultGameModuleImpl
{
    virtual void StartupModule() override
    {
        FDefaultGameModuleImpl::StartupModule();

        // Register property customization
        FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        PropertyModule.RegisterCustomPropertyTypeLayout("SpellParam", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSpellDataCustomization::MakeInstance));
    }

    virtual void ShutdownModule() override
    {
        FDefaultGameModuleImpl::ShutdownModule();

        // Unregister property customization
        if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
        {
            FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
            PropertyModule.UnregisterCustomPropertyTypeLayout("SpellParam");
        }
    }
};


IMPLEMENT_PRIMARY_GAME_MODULE( FSpellcraftGameModule, Spellcraft, "Spellcraft" );

DEFINE_LOG_CATEGORY(LogSpellcraft)
 