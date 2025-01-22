// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MagicBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ESpellDataType : uint8
{
    SpellReference  UMETA(DisplayName = "Spell reference"),
    Boolean         UMETA(DisplayName = "Boolean"),
    Float           UMETA(DisplayName = "Float"),
    String          UMETA(DisplayName = "String")
};

UENUM(BlueprintType)
enum EFormType : uint8
{
    Projectile	UMETA(DisplayName = "Projectile"),
    Field		UMETA(DisplayName = "Field"),
    Beam		UMETA(DisplayName = "Beam"),
    Spawn		UMETA(DisplayName = "Spawn")
};

UENUM(BlueprintType)
enum ESpellProperty : uint8
{
    OnCollide   UMETA(DisplayName = "On collide"),
    OnDeath     UMETA(DisplayName = "On death"),
    Radius      UMETA(DeisplayName = "Radius"),
    None        UMETA(DisplayName = "None"),
};

USTRUCT(BlueprintType)
struct FSpellData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TEnumAsByte<EFormType> Form;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    bool bInitial;

    // Map of dynamic properties using smart pointers for memory management
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpellData")
    TMap<TEnumAsByte<ESpellProperty>, FString> DynamicProperties;
};

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API UMagicBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = Spellcraft)
    static TMap<TEnumAsByte<ESpellProperty>, ESpellDataType> GetSpellPropertyMap()
    {
        TMap<TEnumAsByte<ESpellProperty>, ESpellDataType> PropertyMap;

        PropertyMap.Add(ESpellProperty::OnCollide, ESpellDataType::SpellReference);
        PropertyMap.Add(ESpellProperty::OnDeath, ESpellDataType::SpellReference);
        PropertyMap.Add(ESpellProperty::Radius, ESpellDataType::Float);

        return PropertyMap;
    }

    UFUNCTION(BlueprintCallable, Category = Spellcraft)
    static ESpellProperty GetSpellPropertyFromInt(int32 Value)
    {
        switch (Value)
        {
        case 0: return ESpellProperty::OnCollide;
        case 1: return ESpellProperty::OnDeath;
        case 2: return ESpellProperty::Radius;
        default: return ESpellProperty::None;
        }
    }

    static TArray<TArray<TEnumAsByte<ESpellProperty>>> GetPropetyByForm()
    {
        TArray<TArray<TEnumAsByte<ESpellProperty>>> PropertiesByForm;

        TArray<ESpellProperty> ProjectileArray = { OnCollide, Radius };
        TArray<ESpellProperty> BeamArray = { OnCollide };
        TArray<ESpellProperty> FieldArray = { OnCollide, Radius };
        TArray<ESpellProperty> SpawnArray = { OnDeath };

        for (int i = EFormType::Projectile; i != EFormType::Spawn; i++)
        {
            switch (i)
            {
            case EFormType::Beam: PropertiesByForm.Emplace(BeamArray); break;
            case EFormType::Projectile: PropertiesByForm.Emplace(ProjectileArray); break;
            case EFormType::Field: PropertiesByForm.Emplace(FieldArray); break;
            case EFormType::Spawn: PropertiesByForm.Emplace(SpawnArray); break;
            default:
                break;
            }
        }

        return PropertiesByForm;
    }
};