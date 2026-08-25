// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRedius = 300.0f;
	ExplosionDamge = 35.0f;
	ItemType = "Mine";

	ExploisionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExploisionCollision"));
	ExploisionCollision->InitSphereRadius(ExplosionRedius);
	ExploisionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExploisionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AMineItem::Explode,
		ExplosionDelay,
		false);
}

void AMineItem::Explode()
{
	TArray<AActor*> OverlappingActors;
	ExploisionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDelay,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
	DestroyItem();
}
