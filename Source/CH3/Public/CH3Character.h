// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CH3Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;
	
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	//체력 회복
	UFUNCTION(BlueprintCallable , Category = "Health")
	void AddHealth(float Amount);

	UFUNCTION()
	void BeginPlay()override;

protected:
	//최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	//현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;

	//체력 감소
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 캐릭터의 컨트롤러와 함수와 연결
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

	//게임오버
	UFUNCTION()
	void OnDeath();

	void UpdateOverheadHP();

private:
	float NormalSpeed;
	float SprintSpeedMultiplier;
	float SprintSpeed;
};
