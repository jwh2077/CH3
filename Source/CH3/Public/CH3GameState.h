// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CH3GameState.generated.h"

/**
 * 
 */
UCLASS()
class CH3_API ACH3GameState : public AGameState
{
	GENERATED_BODY()


public:
	ACH3GameState();

	virtual void BeginPlay() override ;

	//점수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	//스폰된 코인 수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawendCoinCount;
	//캐릭터가 먹은 코인수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	//타이머 시간 조절
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	//현재 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	FTimerHandle LevelTimerHandle;


	//점수 확인
	UFUNCTION(BlueprintPure, Category = "Score")
	int GetScore()const;

	//점수 추가
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);

	//게임 종료
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	//게임 시작
	void StartLevel();

	//타이머 종료시 실행
	void OnLevelTimeUp();
	
	//특정 레벨 종료
	void EndLevel();

};
