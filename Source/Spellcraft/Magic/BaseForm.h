// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBlueprintFunctionLibrary.h"
#include "BaseForm.generated.h"

class AProjectileForm;
class ABeamForm;

UCLASS()
class SPELLCRAFT_API ABaseForm : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseForm();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpellCraft, meta = (ExposeOnSpawn = "true"))
	int32 Id;

	float OverlapRadius = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpellCraft, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<AActor> ActorToIgnore;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeSpellData();

	void SpellDestruction();

	// Actor we hit with our spell
	AActor* HitActor;

	// Location where we hit actor
	FVector HitActorLocation;

	FSpellData SpellData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpellCraft)
	TSubclassOf<AProjectileForm> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpellCraft)
	TSubclassOf<ABeamForm> BeamClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
