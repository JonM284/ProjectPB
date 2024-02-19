// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PinBrawlersCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APinBrawlersCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SecondaryAimAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* WackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityTwoAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USceneComponent* wackLocation;

public:
	APinBrawlersCharacter();
	
	void KnockbackPlayer(FVector _knockbackDirection, float _knockbackForce, float _knockbackTime);

	bool bIsKnockedBack;

	UFUNCTION(BlueprintCallable)
	void SetController(AController* _controller);

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterBase, meta = (AllowPrivateAccess = "true"))
	float wackForce = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterBase, meta = (AllowPrivateAccess = "true"))
	float wackCooldownTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterBase, meta = (AllowPrivateAccess = "true"))
	float wackActionTimeMax = 0.08f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterBase, meta = (AllowPrivateAccess = "true"))
	float wackChargeTimeMax = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterBase, meta = (AllowPrivateAccess = "true"))
	float aimDirValueTolerance = 0.15f;

	float m_originalGravityScale;

	float m_wackStrengthChargeAmount;

	bool bCanWack = true;
	

	bool bHasHitBall;

	bool bIsWacking;

	bool bIsChargingWack;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void Aim(const FInputActionValue& Value);

	void SecondaryAim(const FInputActionValue& Value);

	virtual void Tick(float DeltaTime) override;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void ChargeWackBall();

	void WackBall();

	void CheckWackBall();

	void FinishWackCheck();

	void WackBallReset();

	void KnockbackReset();
	
	virtual void AbilityOne();

	virtual void AbilityTwo();

	FVector2D movementInput;

	FVector2D aimDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = CharacterSpecific)
	float wackRangeRad = 200.0f;

};

