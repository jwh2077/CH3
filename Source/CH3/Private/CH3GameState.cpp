// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "SpawnVolume.h"
#include "CH3GameInstance.h"
#include "CH3PlayerController.h"
#include "CoinIteam.h"
#include "Blueprint/UserWidget.h"

ACH3GameState::ACH3GameState()
{
	Score = 0;
	SpawendCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;

	// Wave 기본값
	WaveDuration = 10.0f;
	CurrentWaveIndex = 0;
	MaxWaves = 3;
	ItemsPerWave = 20;

	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ACH3GameState::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (!CurrentMapName.Contains(TEXT("MenuLevel")))
	{
		StartLevel();
	}

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACH3GameState::UpDateHUD,
		0.1f,
		true
	);
}

int ACH3GameState::GetScore()const
{
	return Score;
}
void ACH3GameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			CH3GameInstance->AddToScore(Amount);
		}
	}
}

void ACH3GameState::StartLevel()
{
	// HUD 세팅
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			CH3PlayerController->ShoeGameHUD();
		}
	}
	
	// 레벨 인덱스 동기화
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			CurrentLevelIndex = CH3GameInstance->CurrentLevelIndex;
		}
	}

	// 레벨 타이머 시작
	if (LevelDuration > 0.0f)
	{
		GetWorldTimerManager().ClearTimer(LevelTimerHandle);
		GetWorldTimerManager().SetTimer(
			LevelTimerHandle,
			this,
			&ACH3GameState::OnLevelTimeUp,
			LevelDuration,
			false
		);
	}

	// 웨이브 초기화 및 시작
	CurrentWaveIndex = 0;
	StartWave();
}

void ACH3GameState::OnLevelTimeUp()
{
	// 레벨 시간이 끝나면 레벨 종료 처리
	EndLevel();
}

void ACH3GameState::StartWave()
{
	// 웨이브 시작 시 카운트 초기화
	SpawendCoinCount = 0;
	CollectedCoinCount = 0;

	// 스폰 볼륨 가져오기
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	// ItemsPerWave 만큼 스폰
	for (int32 i = 0; i < ItemsPerWave; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			// 간단하게 첫번째 스폰볼륨을 사용
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawendCoinCount++;
				}
			}
		}
	}

	// 웨이브 타이머 시작
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&ACH3GameState::OnWaveTimeUp,
		WaveDuration,
		false
	);
}

void ACH3GameState::OnWaveTimeUp()
{
	EndWave();
}

void ACH3GameState::EndWave()
{
	// 웨이브 타이머 클리어
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	// 웨이브 종료 시 남아있는 코인만 제거
	ClearRemainingCoins();

	// 다음 웨이브로 진행하거나 레벨 종료
	CurrentWaveIndex++;

	if (CurrentWaveIndex >= MaxWaves)
	{
		// 모든 웨이브 종료 - 레벨 종료 처리
		EndLevel();
	}
	else
	{
		StartWave();
	}
}

void ACH3GameState::EndLevel()
{
	// 레벨/웨이브 타이머 클리어
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	// 레벨 인덱스 증가 및 저장
	CurrentLevelIndex++;

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			AddScore(Score);
			CH3GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels)
	{	
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ACH3GameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			CH3PlayerController->SetPause(true);
			CH3PlayerController->ShowMainMenu(true);
		}
	}
}

void ACH3GameState::OnCoinCollecte()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawendCoinCount);

	// 현재 웨이브의 모든 코인을 수집하면 웨이브 종료
	if (SpawendCoinCount > 0 && CollectedCoinCount >= SpawendCoinCount)
	{
		// 웨이브 타이머 해제 후 EndWave 호출
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		EndWave();
	}
}

void ACH3GameState::ClearRemainingCoins()
{
	// 월드에서 모든 코인 아이템을 찾아서 파괴(코인만 삭제)
	TArray<AActor*> FoundCoins;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACoinItem::StaticClass(), FoundCoins);

	for (AActor* Coin : FoundCoins)
	{
		if (Coin && !Coin->IsPendingKillPending())
		{
			Coin->Destroy();
		}
	}
}

void ACH3GameState::UpDateHUD()
{
	if (APlayerController* PlayerConTroller = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerConTroller = Cast< ACH3PlayerController>(PlayerConTroller))
		{
			if (UUserWidget* UHDWidet = CH3PlayerConTroller->GetHUDWidget())
			{
				// Time 텍스트는 레벨 시간으로 사용
				if (UTextBlock* TimeText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingLevelTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					if (RemainingLevelTime < 0.0f) RemainingLevelTime = 0.0f;
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"), RemainingLevelTime)));
				}

				// WaveTime UI 업데이트
				if (UTextBlock* WaveTimeText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("WaveTime"))))
				{
					float RemainingWaveTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					if (RemainingWaveTime < 0.0f) RemainingWaveTime = 0.0f;
					WaveTimeText->SetText(FText::FromString(FString::Printf(TEXT("Wave Time : %.1f"), RemainingWaveTime)));
				}

				//웨이브 UI 업데이트
				if (UTextBlock* WaveText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d / %d"), CurrentWaveIndex + 1, MaxWaves)));
				}
				else if (UTextBlock* WaveCountText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("WaveCount"))))
				{
					WaveCountText->SetText(FText::FromString(FString::Printf(TEXT("Wave : %d / %d"), CurrentWaveIndex + 1, MaxWaves)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
						if (CH3GameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score : %d"), CH3GameInstance->TotalScore)));
						}
					}
				}
				
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level : %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}