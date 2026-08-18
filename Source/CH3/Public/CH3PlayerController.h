// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CH3PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class CH3_API ACH3PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACH3PlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;

protected:
	virtual void BeginPlay() override;
};
