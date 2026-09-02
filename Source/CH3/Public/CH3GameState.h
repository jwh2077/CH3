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
	//레벨 길이
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	//현재 레벨
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName>LevelMapNames;




	// 한 웨이브 당 지속시간(초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;
	// 현재 웨이브 인덱스 (0-based)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	// 총 웨이브 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 MaxWaves;
	// 한 웨이브 당 스폰할 아이템 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 ItemsPerWave;




	FTimerHandle LevelTimerHandle; // 레벨 타이머
	FTimerHandle WaveTimerHandle;  // 웨이브 타이머
	FTimerHandle HUDUpdateTimerHandle;


	//점수 확인
	UFUNCTION(BlueprintPure, Category = "Score")
	int GetScore()const;
	//점수 추가
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	//게임 종료
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	//게임 시작 (레벨 시작)
	void StartLevel();

	// 레벨 타임업 핸들러
	void OnLevelTimeUp();

	// 웨이브 시작 / 종료 함수들
	void StartWave();
	//타이머 종료시 실행 (웨이브 종료)
	void OnWaveTimeUp();
	//웨이브 종료 처리
	void EndWave();

	//특정 레벨 종료
	void EndLevel();
	//코인 카운터
	void OnCoinCollecte();
	//남아있는 코인만 삭제
	void ClearRemainingCoins();

	//UI업데이트
	void UpDateHUD();

};
