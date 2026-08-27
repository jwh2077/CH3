// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameInstance.h"

UCH3GameInstance::UCH3GameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UCH3GameInstance::AddToScore(int32 Amont)
{
	TotalScore += Amont;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"),TotalScore)
}
