// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseForm.h"
#include "BeamForm.generated.h"

/**
 * 
 */
UCLASS()
class SPELLCRAFT_API ABeamForm : public ABaseForm
{
	GENERATED_BODY()

public:

	ABeamForm();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Beam)
	class UNiagaraComponent* BeamEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Beam)
	class UParticleSystem* ImpactEffect;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(const FHitResult& Hit);
};
