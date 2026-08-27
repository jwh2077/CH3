// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3PlayerController.h"
#include "CH3GameState.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

ACH3PlayerController::ACH3PlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr)
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

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
	ACH3GameState* CH3GameState = GetWorld() ? GetWorld()->GetGameState<ACH3GameState>() : nullptr;
	if (CH3GameState)
	{
		CH3GameState->UpDateHUD();
	}
}

UUserWidget* ACH3PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}
