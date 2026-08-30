// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3PlayerController.h"
#include "CH3GameState.h"
#include "CH3GameInstance.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

ACH3PlayerController::ACH3PlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	HUDWidgetClass(nullptr),
	HUDWidgetInstance(nullptr),
	MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
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
	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* ACH3PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void ACH3PlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("TEST")));
		}

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString("Restart"));
				
			}
			else
			{
				ButtonText->SetText(FText::FromString("Start"));
			}
			if (bIsRestart)
			{
				UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayerGameOverAnim"));
				if (PlayAnimFunc)
				{
					MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
				}
				if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
				{
					if (UCH3GameInstance* CH3GameInstance = Cast< UCH3GameInstance>(UGameplayStatics::GetGameInstance(this)))
					{
						TotalScoreText->SetText(FText::FromString(
							FString::Printf(TEXT("Total Score : %d"),CH3GameInstance->TotalScore)
						));
					}
				}
			}
		}
	}
}

void ACH3PlayerController::ShoeGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}
	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());
		}

		ACH3GameState* CH3GameState = GetWorld() ? GetWorld()->GetGameState<ACH3GameState>() : nullptr;
		if (CH3GameState)
		{
			CH3GameState->UpDateHUD();
		}
	}
}


void ACH3PlayerController::StartGame()
{
	//if (UCH3GameInstance* GameInstance = Cast<UCH3GameInstance> (GetWorld()->GetGameInstance<UCH3GameInstance>()))
	if (UCH3GameInstance* CH3GameInstance = Cast<UCH3GameInstance> (UGameplayStatics::GetGameInstance(this)))
	{
		CH3GameInstance->CurrentLevelIndex = 0;
		CH3GameInstance->TotalScore = 0;
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
	SetPause(false);
}

