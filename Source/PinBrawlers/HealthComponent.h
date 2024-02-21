// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PINBRAWLERS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	bool bShieldCracked;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shield, meta = (AllowPrivateAccess = "true"))
	float shieldMax = 1000;

	float currentShield = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shield, meta = (AllowPrivateAccess = "true"))
	float shieldRegenRate = 4.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Shield, meta = (AllowPrivateAccess = "true"))
	float shieldRegenWaitTimerMax = 4.5f;

	float shieldRegenCurrentTimer;

	bool bCanRegenShield;

	FTimerHandle shieldCrackedTimer;

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* Instigator, AActor* DamageCauser);

	void RegenShield();

	class APinBrawlersGameMode* PinBrawlersGameMode;
};
