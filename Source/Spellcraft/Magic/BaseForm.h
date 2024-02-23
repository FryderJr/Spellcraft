// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseForm.generated.h"

UENUM(BlueprintType)
enum EFormType : uint8
{
	Projectile	UMETA(DisplayName = "Projectile"),
	Field		UMETA(DisplayName = "Field"),
	Beam		UMETA(DisplayName = "Beam"),
	Spawn		UMETA(DisplayName = "Spawn")
};

USTRUCT(BlueprintType)
struct FSpellData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	TEnumAsByte<EFormType> Form;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	bool bInitial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	int32 OnCollide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	int32 OnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	int32 OnDeath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	int32 OnDamageDeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpellData)
	int32 OnDamageRecieve;
};

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

	FSpellData* ReadSpellDataById(int32 SpellId);

	float OverlapRadius = 600.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitializeSpellData();

	virtual void SpawnSpellByID();

	virtual void StartSpellOverlapCheck(FVector& Center);

	virtual void OnOverlapCompleted(const FTraceHandle& Handle, FOverlapDatum& Data);
	
	virtual void OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data);

	// Actor to Ignore
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpellCraft, meta = (ExposeOnSpawn = "true"))
	AActor* ActorToIgnore;

	// Actor we hit with our spell
	AActor* HitActor;

	// Location where we hit actor
	FVector HitActorLocation;

	FSpellData* SpellData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpellCraft)
	TSubclassOf<AProjectileForm> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SpellCraft)
	TSubclassOf<ABeamForm> BeamClass;

	FOverlapDelegate OverlapDelegate;

	FTraceDelegate TraceDelegate;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
