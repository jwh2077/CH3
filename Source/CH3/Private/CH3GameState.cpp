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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerController = Cast<ACH3PlayerController>(PlayerController))
		{
			CH3PlayerController->ShoeGameHUD();
		}
	}
	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance>(GameInstance);
		if (CH3GameInstance)
		{
			CurrentLevelIndex = CH3GameInstance->CurrentLevelIndex;
		}
	}
	SpawendCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
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

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ACH3GameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
	
}

void ACH3GameState::OnLevelTimeUp()
{
	EndLevel();
}

void ACH3GameState::OnCoinCollecte()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collented: %d / %d"),
		CollectedCoinCount,
		SpawendCoinCount);

	if (SpawendCoinCount > 0 && CollectedCoinCount >= SpawendCoinCount)
	{
		EndLevel();
	}
}

void ACH3GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
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

void ACH3GameState::UpDateHUD()
{
	if (APlayerController* PlayerConTroller = GetWorld()->GetFirstPlayerController())
	{
		if (ACH3PlayerController* CH3PlayerConTroller = Cast< ACH3PlayerController>(PlayerConTroller))
		{
			if (UUserWidget* UHDWidet = CH3PlayerConTroller->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(UHDWidet->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time : %.1f"),RemainingTime)));
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