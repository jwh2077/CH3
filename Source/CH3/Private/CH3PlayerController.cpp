// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3PlayerController.h"
#include "EnhancedInputSubsystems.h"

ACH3PlayerController::ACH3PlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr)
{

}

void ACH3PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = 
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
