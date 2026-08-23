// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class CH3_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VALUEVALUE")
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VALUEVALUE")
	UBoxComponent* SpawningBox;

	FVector GetRandomPointInVolume()const;
	void SpwnItem(TSubclassOf<AActor>ItemClass);
};
