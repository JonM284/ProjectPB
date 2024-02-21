// Fill out your copyright notice in the Description page of Project Settings.


#include "KillWalls.h"
#include "PinBrawlersCharacter.h"
#include "Components/BoxComponent.h"
#include "PinBrawlersGameMode.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AKillWalls::AKillWalls()
{
 	playerCheckCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(playerCheckCollider);
}

void AKillWalls::BeginPlay()
{
	Super::BeginPlay();

	playerCheckCollider->OnComponentHit.AddDynamic(this, &AKillWalls::OnHit);	

	pinBrawlersGameMode = Cast<APinBrawlersGameMode>(UGameplayStatics::GetGameMode(this));
}

// Called every frame
void AKillWalls::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	APinBrawlersCharacter* collidedCharacter = Cast<APinBrawlersCharacter>(OtherActor);

	if(!collidedCharacter)
	{
		return;
	}

	if(!collidedCharacter->bIsKnockedBack)
	{
		return;
	}

	//KILL Player if they are being knocked back and touching wall
	pinBrawlersGameMode->PlayerDied(collidedCharacter);

	UE_LOG(LogTemp, Log, TEXT("Collided Character = %s should be dead"), *collidedCharacter->GetName());
}

