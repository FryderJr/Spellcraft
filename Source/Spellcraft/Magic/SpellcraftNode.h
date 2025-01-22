// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicBlueprintFunctionLibrary.h"
#include "SpellcraftNode.generated.h"

UCLASS()
class SPELLCRAFT_API ASpellcraftNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpellcraftNode();

	UPROPERTY(EditDefaultsOnly, Category = Spellcraft)
	TObjectPtr<UStaticMeshComponent> SpellcraftMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spellcraft)
	int32 SpellId = -1;

	UPROPERTY(BlueprintReadWrite, Category = Spellcraft, meta = (ExposeOnSpawn = "true"))
	int32 ParentSpellId = -1;

	TMap<TEnumAsByte<ESpellProperty>, ESpellDataType> SpellDataTypeByProperty;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ClearFromParents(AActor* DestroyedActor);

	ASpellcraftNode* GetParentNode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
