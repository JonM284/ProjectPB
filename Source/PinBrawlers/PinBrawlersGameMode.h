// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PinBrawlersGameMode.generated.h"

class APinBrawlersCharacter;

UCLASS()
class PINBRAWLERS_API APinBrawlersGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	public:

	void PlayerDied(APinBrawlersCharacter* DeadActor);

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Players, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> allPlayers;

	TArray<APinBrawlersCharacter*> deadPlayers;

	AActor* deadPlayerLocation;

	float startDelay = 3.f;

	int32 maxPlayerCount = 0;
	int32 playerCount = 0;

	int32 GetPlayerCount();

	void HandleGameStart();

	void EnableAllPlayerControllers(bool _bIsEnabled);

};
