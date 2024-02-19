// Copyright Epic Games, Inc. All Rights Reserved.

#include "PinBrawlersCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "BallBase.h"
#include "DrawDebugHelpers.h"
#include "Engine/InputDelegateBinding.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APinBrawlersCharacter

APinBrawlersCharacter::APinBrawlersCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	wackLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Wack Start Location"));
	wackLocation->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APinBrawlersCharacter::BeginPlay()
{
	// Call the base class 
	Super::BeginPlay(); 
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	m_originalGravityScale = GetCharacterMovement()->GravityScale;
}

void APinBrawlersCharacter::SetController(AController* _controller){

}

//////////////////////////////////////////////////////////////////////////
// Input

void APinBrawlersCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		UE_LOG(LogTemp, Log, TEXT("Setting Up Inputs"));

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APinBrawlersCharacter::Move);
		//Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &APinBrawlersCharacter::Aim);
		EnhancedInputComponent->BindAction(SecondaryAimAction, ETriggerEvent::Triggered, this, &APinBrawlersCharacter::SecondaryAim);

		//Wack
		EnhancedInputComponent->BindAction(WackAction, ETriggerEvent::Triggered, this, &APinBrawlersCharacter::WackBall);

		//Abilities
		EnhancedInputComponent->BindAction(AbilityOneAction, ETriggerEvent::Triggered, this, &APinBrawlersCharacter::AbilityOne);
		EnhancedInputComponent->BindAction(AbilityTwoAction, ETriggerEvent::Triggered, this, &APinBrawlersCharacter::AbilityTwo);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	if(Controller != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Controller isn't null -> name:%s"), *Controller->GetName());
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("Controller NULL"));
	}
}

void APinBrawlersCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsChargingWack && m_wackStrengthChargeAmount < wackChargeTimeMax)
	{
		m_wackStrengthChargeAmount += DeltaTime;
	}

	//set the time to be able to hit the ball slightly larger
	if(bIsWacking)
	{
		CheckWackBall();
	}

}

void APinBrawlersCharacter::Move(const FInputActionValue& Value)
{
	if(bIsKnockedBack)
	{
		return;
	}

	// input is a Vector2D
	movementInput = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, movementInput.Y);
		AddMovementInput(RightDirection, movementInput.X);
	}
}

void APinBrawlersCharacter::Aim(const FInputActionValue& Value)
{
	if(bIsKnockedBack)
	{
		return;
	}

	aimDirection = Value.Get<FVector2D>();
}

void APinBrawlersCharacter::SecondaryAim(const FInputActionValue& Value)
{
	if(bIsKnockedBack)
	{
		return;
	}

	if(aimDirection.GetAbs() <= FVector2D(aimDirValueTolerance, aimDirValueTolerance))
	{
		aimDirection = Value.Get<FVector2D>();
	}
}

void APinBrawlersCharacter::ChargeWackBall(){
	if(!bCanWack || bIsKnockedBack){
		return;
	}

	bIsChargingWack = true;
}

void APinBrawlersCharacter::WackBall(){
	if(!bCanWack || bIsKnockedBack){
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("WACK BALL => START"));
	bIsChargingWack = false;
	bCanWack = false;

	//Perform Wack Ball Check
	bIsWacking = true;
	bHasHitBall = false;

	
	//Wack action check timer
	FTimerHandle wackActionTimer;
	FTimerDelegate wackActionEndDelegate = FTimerDelegate::CreateUObject(this, &APinBrawlersCharacter::FinishWackCheck);
	GetWorldTimerManager().SetTimer(wackActionTimer, wackActionEndDelegate, wackActionTimeMax, false);

	//Cooldown timer
	FTimerHandle wackResetTimer;
	FTimerDelegate wackEndDelegate = FTimerDelegate::CreateUObject(this, &APinBrawlersCharacter::WackBallReset);
	GetWorldTimerManager().SetTimer(wackResetTimer, wackEndDelegate, wackCooldownTime, false);
}

void APinBrawlersCharacter::CheckWackBall()
{
	if(bHasHitBall)
	{
		return;
	}

	//Debug
	UE_LOG(LogTemp, Log, TEXT("Checking Wack Ball"));
	
	DrawDebugSphere(GetWorld(),  wackLocation->GetComponentLocation(), wackRangeRad, 18, FColor::Red, false, 2.0f);
	
	FVector lineEnd = wackLocation->GetComponentLocation() + FVector(aimDirection.X, aimDirection.Y, 0) * 100;
	
	DrawDebugLine(GetWorld(), wackLocation->GetComponentLocation(), lineEnd, FColor::Green, false, 5.0f);
	
	//Sphere Trace
	FCollisionShape wackSphere = FCollisionShape::MakeSphere(wackRangeRad);
	
	FHitResult HitResult;

	if(GetWorld()->SweepSingleByChannel(HitResult, wackLocation->GetComponentLocation(), wackLocation->GetComponentLocation(),
	 FQuat::Identity, ECC_GameTraceChannel1, wackSphere))
	{
		ABallBase* ball = Cast<ABallBase>(HitResult.GetActor());
		if(ball)
		{
			//Wack Ball
			UE_LOG(LogTemp, Log, TEXT("FOUND BALL"));
			ball->WackBall(FVector(aimDirection.X, aimDirection.Y, 0), wackForce, this);
			bHasHitBall = true;
		} 
	}
}

void APinBrawlersCharacter::FinishWackCheck()
{
	UE_LOG(LogTemp, Log, TEXT("WACK BALL => END XX"));
	bIsWacking = false;
}

void APinBrawlersCharacter::WackBallReset()
{
	bCanWack = true;	
}

void APinBrawlersCharacter::KnockbackPlayer(FVector _knockbackDirection, float _knockbackForce, float _knockbackTime)
{
	//Check current shields, if shields are depleted => DOUBLE knockback force
	LaunchCharacter(_knockbackDirection.GetSafeNormal() * _knockbackForce, true, true);

	bIsKnockedBack = true;

	FTimerHandle knockbackTimer;
	FTimerDelegate knockbackEndDelegate = FTimerDelegate::CreateUObject(this, &APinBrawlersCharacter::KnockbackReset);
	GetWorldTimerManager().SetTimer(knockbackTimer, knockbackEndDelegate, _knockbackTime, false);
}

void APinBrawlersCharacter::KnockbackReset(){
	bIsKnockedBack = false;
}

void APinBrawlersCharacter::AbilityOne(){
	if(bIsKnockedBack){
		return;
	}
	//TBD on other characters
}

void APinBrawlersCharacter::AbilityTwo(){
	if(bIsKnockedBack){
		return;
	}
	//TBD on other characters
}