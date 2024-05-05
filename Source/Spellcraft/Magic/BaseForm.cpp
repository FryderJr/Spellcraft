// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseForm.h"
#include "BeamForm.h"
#include "ProjectileForm.h"
#include "Spellcraft/SpellcraftState.h"
#include "Spellcraft/SpellCraftGameInstance.h"
#include "GameFramework/ProjectileMovementComponent.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,text)

// Sets default values
ABaseForm::ABaseForm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*
	}*/
}

// Called when the game starts or when spawned
void ABaseForm::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSpellData();
}

void ABaseForm::InitializeSpellData()
{
	ASpellcraftState* SpellCraftGameState = Cast<ASpellcraftState>(GetWorld()->GetGameState());
	if (!SpellCraftGameState) {
		return;
	}

	SpellData = SpellCraftGameState->ReadSpellDataById(Id);
}

void ABaseForm::SpellDestruction()
{
	SetLifeSpan(0.1f);
}

// Called every frame
void ABaseForm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

