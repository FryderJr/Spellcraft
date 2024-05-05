// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <atomic>
#include "CoreMinimal.h"
#include "MagicEffect.h"
#include "TargetAroundEffect.generated.h"

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API UTargetAroundEffect : public UMagicEffect
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, Category = SpellCraft)
	FVector TargetCenter;

	UPROPERTY(BlueprintReadWrite, Category = SpellCraft)
	float EffectRadius = 600.0f;

	UPROPERTY(BlueprintReadWrite, Category = SpellCraft)
	AActor* HitActor;

	virtual void PostInitProperties() override;

	virtual void Execute(TFunction<void()> OnCommandCompleted) override;

private:

	std::atomic<int> PendingOperations{ 0 };

	FOverlapDelegate OverlapDelegate;

	FTraceDelegate TraceDelegate;

	void OnOverlapCompleted(const FTraceHandle& Handle, FOverlapDatum& Data);

	void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);
	
};
