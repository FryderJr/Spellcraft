// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellcraftConnection.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ASpellcraftConnection::ASpellcraftConnection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(FName("SplineComponent"));
    SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(FName("SplineMesh"));
    SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
    SplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
}

// Called when the game starts or when spawned
void ASpellcraftConnection::BeginPlay()
{
	Super::BeginPlay();
	
    UStaticMesh* Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, TEXT("/Engine/EngineMeshes/Cylinder.Cylinder")));
    SplineMesh->SetStaticMesh(Mesh);
    SplineMesh->SetForwardAxis(ESplineMeshAxis::Y);
}

// Called every frame
void ASpellcraftConnection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpellcraftConnection::UpdateConnection(AActor* NodeA, AActor* NodeB)
{
    if (NodeA && NodeB)
    {
        FVector StartPoint = NodeA->GetActorLocation();
        FVector EndPoint = NodeB->GetActorLocation();

        // Устанавливаем начало и конец сплайна
        SplineComponent->ClearSplinePoints();
        SplineComponent->AddSplinePoint(StartPoint, ESplineCoordinateSpace::World);
        SplineComponent->AddSplinePoint(EndPoint, ESplineCoordinateSpace::World);
        SplineComponent->SetSplinePointType(0, ESplinePointType::Linear);
        SplineComponent->SetSplinePointType(1, ESplinePointType::Linear);

        FVector StartPos, StartTangent, EndPos, EndTangent;
        SplineComponent->GetLocationAndTangentAtSplinePoint(0, StartPos, StartTangent, ESplineCoordinateSpace::World);
        SplineComponent->GetLocationAndTangentAtSplinePoint(1, EndPos, EndTangent, ESplineCoordinateSpace::World);

        SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
        SplineMesh->SetStartScale(FVector2D(0.05, 0.05));
        SplineMesh->SetEndScale(FVector2D(0.05, 0.05));
    }
}

