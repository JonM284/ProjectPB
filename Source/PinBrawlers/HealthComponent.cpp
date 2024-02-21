// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	currentShield = shieldMax;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bCanRegenShield && currentShield < shieldMax)
	{
		currentShield += DeltaTime * shieldRegenRate;
	}else if(bCanRegenShield && currentShield >= shieldMax){
		bCanRegenShield = false;
	}

}

void UHealthComponent::DamageTaken(AActor *DamagedActor, float Damage, const UDamageType *DamageType, class AController *Instigator, AActor *DamageCauser)
{
	if(Damage <= 0)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Shield BEFORE: %f"), currentShield);

	UE_LOG(LogTemp, Log, TEXT("Damage: %f"), Damage);
	currentShield -= Damage;
	FMath::Clamp(currentShield, 0, shieldMax);

	UE_LOG(LogTemp, Log, TEXT("Shield AFTER: %f"), currentShield);

	if(currentShield <= 0)
	{
		bShieldCracked = true;
	
		//CLEAR timer if they get hit again before the regen wait timer is over
		if(GetWorld()->GetTimerManager().IsTimerActive(shieldCrackedTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(shieldCrackedTimer);
		}
		
		//Set Timer for player to wait to start regenerating shield
		FTimerDelegate crackedTimerEndDelegate = FTimerDelegate::CreateUObject(this, &UHealthComponent::RegenShield);
		GetWorld()->GetTimerManager().SetTimer(shieldCrackedTimer, crackedTimerEndDelegate, shieldRegenWaitTimerMax, false);
	}

}

void UHealthComponent::RegenShield()
{
	bShieldCracked = false;
	bCanRegenShield = true;
}




