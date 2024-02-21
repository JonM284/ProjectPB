// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillWalls.generated.h"


UCLASS()
class PINBRAWLERS_API AKillWalls : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AKillWalls();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class APinBrawlersGameMode* pinBrawlersGameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* playerCheckCollider;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit);

};
