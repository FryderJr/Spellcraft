// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MagicEffect.generated.h"

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API UMagicEffect : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = SpellCraft)
	int32 Id;

	virtual void Execute(TFunction<void()> OnCommandCompleted);

protected:

	TFunction<void()> CommandCompletedCallback;
	
};
