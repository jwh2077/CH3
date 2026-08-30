// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

// Sets default values
AItem::AItem()
{
	SceneRoot = CreateDefaultSubobject <USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMashComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMash"));
	StaticMashComp->SetupAttachment(SceneRoot);

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Resources/Props/SM_Star_C.SM_Star_C"));
	//if (MeshAsset.Succeeded())
	//{
	//	StaticMashComp->SetStaticMesh(MeshAsset.Object);
	//}
	//static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/Resources/Materials/M_Coin_A.M_Coin_A"));
	//if (MaterialAsset.Succeeded())
	//{
	//	StaticMashComp->SetMaterial(0, MaterialAsset.Object);
	//}
	PrimaryActorTick.bCanEverTick = true;
	RotationSpeed = 90.0f;
	ZSpeed = 12;
	MaxZ = 6.0f;
	MinZ = -6.0f;
	Z = 0;
	UP = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{	
	Super::Tick(DeltaTime);
	float UPZ = ZSpeed * DeltaTime;
	if (!FMath::IsNearlyZero( RotationSpeed ))
	{
		AddActorLocalRotation(FRotator(RotationSpeed * DeltaTime, 0.0f, 0.0f));
	}
	if (Z >= MaxZ)
	{
		UP = false;
	}
	else if (Z <= MinZ)
	{
		UP = true;
	}
	if (UP)
	{
		AddActorWorldOffset(FVector(0.0f, 0.0f, UPZ));
		Z += UPZ;
	}
	else if (!UP)
	{
		AddActorWorldOffset(FVector(0.0f, 0.0f, -UPZ));
		Z += -UPZ;
	}
}

void AItem::ResetActorPosition( )
{
	SetActorLocation(FVector::ZeroVector);
}

float AItem::GetRotationSpeed( ) const
{
	return RotationSpeed;
}