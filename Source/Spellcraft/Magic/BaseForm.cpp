// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseForm.h"
#include "BeamForm.h"
#include "ProjectileForm.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Spellcraft/SpellCraftGameInstance.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red,text)

// Sets default values
ABaseForm::ABaseForm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/*
	}*/
}

FSpellData* ABaseForm::ReadSpellDataById(int32 SpellId)
{
	USpellCraftGameInstance* GameInstance = Cast<USpellCraftGameInstance>(GetGameInstance());
	FSpellData* TempSpellData = nullptr;
	if (GameInstance->SpellDataTable)
	{
		static const FString ContextString(TEXT("My Data Table Context"));
		FString NameString = FString::FromInt(SpellId);
		FName RowName = FName(*NameString);

		TempSpellData = GameInstance->SpellDataTable->FindRow<FSpellData>(RowName, ContextString);
	}
	return TempSpellData;
}

// Called when the game starts or when spawned
void ABaseForm::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSpellData();

	OverlapDelegate.BindUObject(this, &ABaseForm::OnOverlapCompleted);
	TraceDelegate.BindUObject(this, &ABaseForm::OnTraceCompleted);
}

void ABaseForm::InitializeSpellData()
{
	SpellData = ReadSpellDataById(Id);
}

void ABaseForm::SpawnSpellByID()
{

}

void ABaseForm::StartSpellOverlapCheck(FVector& Center)
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	FCollisionQueryParams CollisionQueryParams;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(OverlapRadius);
	DrawDebugSphere(GetWorld(), Center, OverlapRadius, 16, FColor::Red, false, 10.0f, 2.0f);
	GetWorld()->AsyncOverlapByObjectType(Center, FQuat(), ObjectQueryParams, CollisionShape, CollisionQueryParams, &OverlapDelegate);
}

void ABaseForm::OnOverlapCompleted(const FTraceHandle& Handle, FOverlapDatum& Data)
{
	print(FString::Printf(TEXT("Actors around hit points %d"), Data.OutOverlaps.Num()));

	//  онтейнер дл€ отслеживани€ обработанных акторов
	TSet<AActor*> ProcessedActors;

	for (const auto& Result : Data.OutOverlaps)
	{
		AActor* ResultActor = Result.GetActor();
		if (ResultActor)
		{
			// ѕровер€ем, был ли этот актор уже обработан
			if (ProcessedActors.Contains(ResultActor))
			{
				// ѕропускаем повторно обработанных акторов
				continue;
			}

			// ƒобавл€ем актора в обработанные
			ProcessedActors.Add(ResultActor);

			print(FString::Printf(TEXT("Actor around hit points name is %s"), *ResultActor->GetName()));

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
			FCollisionQueryParams CollisionQueryParams;
			CollisionQueryParams.AddIgnoredActor(this);

			DrawDebugLine(GetWorld(), HitActorLocation, ResultActor->GetActorLocation(), FColor::Blue, false, 5.0f, 0, 1.0f);

			// «десь стоит уточнить, что HitActor или HitActorLocation должны быть корректно определены
			GetWorld()->AsyncLineTraceByObjectType(EAsyncTraceType::Single, HitActorLocation, ResultActor->GetActorLocation(), ObjectQueryParams, CollisionQueryParams, &TraceDelegate);
		}
	}
}

void ABaseForm::OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	FSpellData* TempSpellData = ReadSpellDataById(SpellData->OnCollide);
	if (!TempSpellData)
	{
		return;
	}

	print(FString::Printf(TEXT("Trace hit points %d"), Data.OutHits.Num()));

	if (Data.OutHits.Num() > 0)
	{
		return;
	}

	FTransform SpawnTransform;

	SpawnTransform.SetLocation(HitActorLocation);
	SpawnTransform.SetRotation((Data.End - SpawnTransform.GetLocation()).Rotation().Quaternion());

	ABaseForm* Magic = nullptr;

	if (TempSpellData->Form == EFormType::Projectile)
	{
		Magic = GetWorld()->SpawnActorDeferred<AProjectileForm>(ProjectileClass, SpawnTransform);
		AProjectileForm* MagicProjectile = Cast<AProjectileForm>(Magic);
		if (MagicProjectile)
		{
			MagicProjectile->ProjectileMovement->Velocity = (Data.End - SpawnTransform.GetLocation()).GetSafeNormal() * MagicProjectile->ProjectileMovement->InitialSpeed;
		}
	}

	if (TempSpellData->Form == EFormType::Beam)
	{
		Magic = GetWorld()->SpawnActorDeferred<ABeamForm>(ABeamForm::StaticClass(), SpawnTransform);
	}

	Magic->Id = SpellData->OnCollide;
	Magic->ActorToIgnore = HitActor;
	Magic->FinishSpawning(SpawnTransform);
}

// Called every frame
void ABaseForm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

