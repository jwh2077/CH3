// Fill out your copyright notice in the Description page of Project Settings.


#include "CH3GameMode.h"
#include "CH3Character.h"
#include "CH3PlayerController.h"
#include "CH3GameState.h"

ACH3GameMode::ACH3GameMode()
{
	DefaultPawnClass = ACH3Character::StaticClass();
	PlayerControllerClass = ACH3PlayerController::StaticClass();
	GameStateClass = ACH3GameState::StaticClass();
}
 