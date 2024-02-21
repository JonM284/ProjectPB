// Fill out your copyright notice in the Description page of Project Settings.


#include "BallBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"
#include "PinBrawlersCharacter.h"

// Sets default values
ABallBase::ABallBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	baseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Base Mesh"));
	RootComponent = baseSphere;

	playerCheckCollider = CreateDefaultSubobject<UShapeComponent>(TEXT("Player Check Collider"));

}

// Called when the game starts or when spawned
void ABallBase::BeginPlay()
{
	Super::BeginPlay();

	baseSphere->OnComponentHit.AddDynamic(this, &ABallBase::OnHit);	
	baseSphere->OnComponentBeginOverlap.AddDynamic(this, &ABallBase::OnOverlapBegin);

	currentMoveSpeed = minMoveSpeed;
}

// Called every frame
void ABallBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsMoving)
	{
		AddActorLocalOffset(moveDirection * currentMoveSpeed * DeltaTime, true);
	}

}

void ABallBase::WackBall(FVector _wackDirection, float _wackForceToAdd, AActor* _wackingPlayer)
{
	if(!bIsMoving){
		bIsMoving = true;
	}

	lastHitPlayer = _wackingPlayer;
	UE_LOG(LogTemp, Log, TEXT("_wackDirection not safe = %s"), *_wackDirection.ToString());
	UE_LOG(LogTemp, Log, TEXT("_wackDirection safe = %s"), *_wackDirection.GetSafeNormal().ToString());
	UE_LOG(LogTemp, Log, TEXT("Current speed BEFORE = %f"), currentMoveSpeed);
	UE_LOG(LogTemp, Log, TEXT("Wack Force = %f"), _wackForceToAdd);



	//ToDo: add charge mechanic
	moveDirection = _wackDirection.GetSafeNormal();
	float tempNewMoveSpeed = currentMoveSpeed + _wackForceToAdd;
	currentMoveSpeed = FMath::Clamp(tempNewMoveSpeed, minMoveSpeed, maxMoveSpeed);
	
	UE_LOG(LogTemp, Log, TEXT("Current speed after = %f"), currentMoveSpeed);
	//Change ball color to player color
}

void ABallBase::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	FVector tempDirection = UKismetMathLibrary::GetReflectionVector(moveDirection, hit.ImpactNormal);
	moveDirection = tempDirection;
}

void ABallBase::OnOverlapBegin(UPrimitiveComponent *OverlappedComponent ,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if(!bIsMoving)
	{
		return;
	}

	//Don't do anything if it was the last player to hit the ball
	if(OtherActor == lastHitPlayer)
	{
		UE_LOG(LogTemp, Log, TEXT("+++ Hit lastHitPlayer +++"));

		return;
	}

	UE_LOG(LogTemp, Log, TEXT(">>> Hit Other Character <<<"));

	APinBrawlersCharacter* brawlerCharacter = Cast<APinBrawlersCharacter>(OtherActor);
	if(brawlerCharacter)
	{
		AController* instigatorController = lastHitPlayer->GetInstigatorController();
		UClass* damageTypeClass = UDamageType::StaticClass();
		
		//Deal damage as percent of current move speed
		float prct = currentMoveSpeed / maxMoveSpeed;
		float currentDamage = maxDamage * prct;
		UGameplayStatics::ApplyDamage(OtherActor, currentDamage, instigatorController, this, damageTypeClass);

		//knockback player
		brawlerCharacter->KnockbackPlayer(
		FVector(OtherActor->GetActorLocation().X - this->GetActorLocation().X, OtherActor->GetActorLocation().Y - this->GetActorLocation().Y, 0), 
		currentMoveSpeed,
		0.25f);
	} 


}


