// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CH3GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CH3_API UCH3GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCH3GameInstance();

	//전체 점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	//현재 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amont);
};
