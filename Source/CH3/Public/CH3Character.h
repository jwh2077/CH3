// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CH3Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class CH3_API ACH3Character : public ACharacter
{
	GENERATED_BODY()

public:
	ACH3Character();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera");
	UCameraComponent* CameraComp;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StarSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};
