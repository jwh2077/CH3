// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "CH3Character.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	{
		if (Activator && Activator->ActorHasTag("Player"))
		{
			if (ACH3Character* PlayerCharacter = Cast< ACH3Character>(Activator))
			{
				PlayerCharacter->AddHealth(HealAmount);
			}

			DestroyItem();
		}
	}
}
