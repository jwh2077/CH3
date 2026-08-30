// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseIteam.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class CH3_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AMineItem();

	USphereComponent* ExploisionCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionDelay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ExplosionRedius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ExplosionDamge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	UParticleSystem* ExplosionParticle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
	USoundBase* ExplosionSound;

	FTimerHandle ExplosionTimerHandle;

	bool bIsExploding;
	
	virtual void ActivateItem(AActor* Activator) override;

	void Explode();
};
