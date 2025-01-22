// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetAroundEffect.h"
#include "BaseForm.h"
#include "BeamForm.h"
#include "ProjectileForm.h"
#include "Spellcraft/SpellcraftState.h"
#include "GameFramework/ProjectileMovementComponent.h"


#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, text)


void UTargetAroundEffect::PostInitProperties()
{
	Super::PostInitProperties();

	OverlapDelegate.BindUObject(this, &UTargetAroundEffect::OnOverlapCompleted);
	TraceDelegate.BindUObject(this, &UTargetAroundEffect::OnTraceCompleted);
}

void UTargetAroundEffect::Execute(TFunction<void()> OnCommandCompleted)
{
	Super::Execute(OnCommandCompleted);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(HitActor);
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(EffectRadius);
	GetWorld()->AsyncOverlapByObjectType(TargetCenter, FQuat(), ObjectQueryParams, CollisionShape, CollisionQueryParams, &OverlapDelegate);
}

void UTargetAroundEffect::OnOverlapCompleted(const FTraceHandle& Handle, FOverlapDatum& Data)
{
	print(FString::Printf(TEXT("Actors around hit points %d"), Data.OutOverlaps.Num()));

	// Контейнер для отслеживания обработанных акторов
	TSet<AActor*> ProcessedActors;

	PendingOperations = Data.OutOverlaps.Num();

	for (const auto& Result : Data.OutOverlaps)
	{
		AActor* ResultActor = Result.GetActor();
		if (ResultActor)
		{
			print(FString::Printf(TEXT("Actor around hit points name is %s"), *ResultActor->GetName()));
			// Проверяем, был ли этот актор уже обработан
			if (ProcessedActors.Contains(ResultActor))
			{
				// Пропускаем повторно обработанных акторов
				continue;
			}

			// Добавляем актора в обработанные
			ProcessedActors.Add(ResultActor);

			FCollisionObjectQueryParams ObjectQueryParams;
			ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
			FCollisionQueryParams CollisionQueryParams;

			// Здесь стоит уточнить, что HitActor или HitActorLocation должны быть корректно определены
			GetWorld()->AsyncLineTraceByObjectType(EAsyncTraceType::Single, TargetCenter, ResultActor->GetActorLocation(), ObjectQueryParams, CollisionQueryParams, &TraceDelegate);
		}
	}
}

void UTargetAroundEffect::OnTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	ASpellcraftState* SpellCraftGameState = Cast<ASpellcraftState>(GetWorld()->GetGameState());
	if (!SpellCraftGameState) {
		if (--PendingOperations == 0)
		{
			// Если все операции завершены, вызываем OnCommandCompleted
			CommandCompletedCallback();
		}
		return;
	}

	FSpellData* TempSpellData = new FSpellData();
	bool Result = SpellCraftGameState->ReadSpellDataById(Id, *TempSpellData);

	if (!Result)
	{
		if (--PendingOperations == 0)
		{
			// Если все операции завершены, вызываем OnCommandCompleted
			CommandCompletedCallback();
		}
		return;
	}

	if (Data.OutHits.Num() > 0)
	{
		if (--PendingOperations == 0)
		{
			// Если все операции завершены, вызываем OnCommandCompleted
			CommandCompletedCallback();
		}
		return;
	}

	FTransform SpawnTransform;

	SpawnTransform.SetLocation(TargetCenter);
	SpawnTransform.SetRotation((Data.End - TargetCenter).Rotation().Quaternion());

	ABaseForm* Magic = nullptr;

	if (TempSpellData->Form == EFormType::Projectile)
	{
		Magic = GetWorld()->SpawnActorDeferred<AProjectileForm>(AProjectileForm::StaticClass(), SpawnTransform);
	}

	if (TempSpellData->Form == EFormType::Beam)
	{
		Magic = GetWorld()->SpawnActorDeferred<ABeamForm>(ABeamForm::StaticClass(), SpawnTransform);
	}

	Magic->Id = Id;
	Magic->ActorToIgnore = HitActor;
	Magic->FinishSpawning(SpawnTransform);

	if (--PendingOperations == 0)
	{
		// Если все операции завершены, вызываем OnCommandCompleted
		CommandCompletedCallback();
	}
}
