// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpellProperties.generated.h"

class UScrollBox;
class USpellPropertyWidget;
class ASpellcraftNode;
class USpellReferenceWidget;

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API USpellProperties : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = Spellcraft)
    void Redraw(ASpellcraftNode* Node);

protected:

    /** Прокручиваемый список свойств */
    UPROPERTY(meta = (BindWidget))
    UScrollBox* PropertyScrollBox;

    UPROPERTY(EditDefaultsOnly, Category = Spellcraft)
    TSubclassOf<USpellReferenceWidget> SpellPropertyWidgetClass;

    UPROPERTY()
    ASpellcraftNode* SpellcraftNode;

private:

    void DeleteProperties();
};
