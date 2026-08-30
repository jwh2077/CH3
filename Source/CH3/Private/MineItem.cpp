// Fill out your copyright notice in the Description page of Project Settings.


#include "MineItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AMineItem::AMineItem()
{
	ExplosionDelay = 5.0f;
	ExplosionRedius = 300.0f;
	ExplosionDamge = 35.0f;
	ItemType = "Mine";
	bIsExploding = false;

	ExploisionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExploisionCollision"));
	ExploisionCollision->InitSphereRadius(ExplosionRedius);
	ExploisionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExploisionCollision->SetupAttachment(Scene);
}

void AMineItem::ActivateItem(AActor* Activator)
{
	if (bIsExploding) return;

	Super::ActivateItem(Activator);

		GetWorld()->GetTimerManager().SetTimer(
			ExplosionTimerHandle,
			this,
			&AMineItem::Explode,
			ExplosionDelay,
			false
		);

		bIsExploding = true;
}

void AMineItem::Explode()
{
	UParticleSystemComponent* Particle = nullptr;
	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		); 
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation (
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors;
	ExploisionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamge,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}
	DestroyItem();

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			2.0f,
			false
		);
	}
}
