// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinIteam.h"

ACH3GameState::ACH3GameState()
{
	Score = 0;
	SpawendCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 0;
}

void ACH3GameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
}

int ACH3GameState::GetScore()const
{
	return Score;
}
void ACH3GameState::AddScore(int32 Amount)
{
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void ACH3GameState::StartLevel()
{
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
	
	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawnd %d coin"), CurrentLevelIndex + 1, SpawendCoinCount);
}

void ACH3GameState::OnLevelTimeUp()
{
	EndLevel();
}

void ACH3GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	CurrentLevelIndex++;

	if (CurrentLevelIndex > MaxLevels)
	{
		OnGameOver();
	}
}

void ACH3GameState::OnGameOver()
{
}
