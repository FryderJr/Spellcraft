// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellcraftNode.h"
#include "Spellcraft/ConstructState.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MagicBlueprintFunctionLibrary.h"
#include "SpellcraftConnection.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Blue,text)

// Sets default values
ASpellcraftNode::ASpellcraftNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpellcraftMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	SpellcraftMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SpellcraftMesh->SetupAttachment(RootComponent);

	SpellDataTypeByProperty = UMagicBlueprintFunctionLibrary::GetSpellPropertyMap();
}

// Called when the game starts or when spawned
void ASpellcraftNode::BeginPlay()
{
	Super::BeginPlay();

	print(FString::Printf(TEXT("Spell Id %d"), SpellId));

	auto GameStateCurrent = Cast<AConstructState>(GetWorld()->GetGameState());
	if (GameStateCurrent)
	{
		SpellId = GameStateCurrent->AddSpellData();
	}

	OnDestroyed.AddDynamic(this, &ASpellcraftNode::ClearFromParents);

	ASpellcraftNode* ParentNode = GetParentNode();

	if (ParentNode)
	{
		FActorSpawnParameters Params;
		ASpellcraftConnection* Connection = GetWorld()->SpawnActor<ASpellcraftConnection>(GetActorLocation(), GetActorRotation(), Params);
		if (Connection)
		{
			Connection->UpdateConnection(this, ParentNode);
		}
	}
}

void ASpellcraftNode::ClearFromParents(AActor* DestroyedActor)
{
	auto GameStateCurrent = Cast<AConstructState>(GetWorld()->GetGameState());

	if (GameStateCurrent)
	{
		FSpellData SpellData;
		GameStateCurrent->ReadSpellDataById(ParentSpellId, SpellData);
		if (!SpellData.DynamicProperties.IsEmpty())
		{
			for (auto it = SpellData.DynamicProperties.begin(); it != SpellData.DynamicProperties.end(); ++it)
			{
				if (SpellDataTypeByProperty[it.Key()] == ESpellDataType::SpellReference)
				{
					SpellData.DynamicProperties.Remove(it.Key());
				}
			}
		}
	}
}

ASpellcraftNode* ASpellcraftNode::GetParentNode()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpellcraftNode::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASpellcraftNode* Node = Cast<ASpellcraftNode>(Actor);
		if (Node)
		{
			if (Node->SpellId == ParentSpellId)
			{
				return Node;
			}
		}
	}
	return nullptr;
}

// Called every frame
void ASpellcraftNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

