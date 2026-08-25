// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinIteam.h"
#include "Engine/World.h"
#include "CH3GameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (UWorld* World = GetWorld())
		{
			if (ACH3GameState* GameState = World->GetGameState<ACH3GameState>())
			{
				GameState->AddScore(PointValue);
			}
		}

		DestroyItem();
	}
}
