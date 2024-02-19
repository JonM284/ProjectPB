// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallBase.generated.h"

UCLASS()
class PINBRAWLERS_API ABallBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* baseSphere;

	AActor* lastHitPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UShapeComponent* playerCheckCollider;
	
public:	
	// Sets default values for this actor's properties
	ABallBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FVector moveDirection;

	float currentMoveSpeed;

	UPROPERTY(EditAnywhere)
	float maxMoveSpeed = 100000.0f;

	UPROPERTY(EditAnywhere)
	float minMoveSpeed = 100.0f;

	bool bIsMoving;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void WackBall(FVector _wackDirection, float _wackForceToAdd, AActor* _wackingPlayer);

};
