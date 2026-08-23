// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseIteam.h"
#include "CoinIteam.generated.h"

/**
 * 
 */
UCLASS()
class CH3_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	

public:
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};
