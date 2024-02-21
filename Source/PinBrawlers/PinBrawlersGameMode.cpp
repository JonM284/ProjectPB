// Fill out your copyright notice in the Description page of Project Settings.


#include "PinBrawlersGameMode.h"
#include "PinBrawlersCharacter.h"
#include "PinBrawlersPlayerController.h"
#include "CameraBehavior.h"
#include "Kismet/GameplayStatics.h"


void APinBrawlersGameMode::PlayerDied(APinBrawlersCharacter* _deadPlayer)
{
	deadPlayers.Add(_deadPlayer);
	playerCount--;
	_deadPlayer->OnPlayerDeath();

	if(playerCount <= 1)
	{
		UE_LOG(LogTemp, Log, TEXT("------ RESTARTING ROUND -------"));
		
	}
}

void APinBrawlersGameMode::BeginPlay()
{
    Super::BeginPlay();

	deadPlayerLocation = GetWorld()->SpawnActor<AActor>(AActor::StaticClass(), FVector(-1000,-1000,-1000), FRotator(0,0,0));

    HandleGameStart();
}

void APinBrawlersGameMode::HandleGameStart()
{
    StartGame();
    
	maxPlayerCount = GetPlayerCount();
    
	playerCount = maxPlayerCount;

	EnableAllPlayerControllers(false);

	FTimerHandle PlayerEnableTimerHandle;
    FTimerDelegate timerEndDelegate = FTimerDelegate::CreateUObject(this, &APinBrawlersGameMode::EnableAllPlayerControllers, true);
    GetWorldTimerManager().SetTimer(PlayerEnableTimerHandle, timerEndDelegate, startDelay, false);
}

int32 APinBrawlersGameMode::GetPlayerCount(){
    UGameplayStatics::GetAllActorsOfClass(this, APinBrawlersCharacter::StaticClass(), allPlayers);
    return allPlayers.Num();
}

void APinBrawlersGameMode::EnableAllPlayerControllers(bool _bIsEnabled)
{
	if(allPlayers.Num() == 0)
	{
		return;
	}

	for (size_t i = 0; i < allPlayers.Num(); i++)
	{
		APinBrawlersPlayerController* _currentController = Cast<APinBrawlersPlayerController>(UGameplayStatics::GetPlayerController(this, i));

		if(_currentController != nullptr)
		{
			_currentController->SetPlayerEnableState(_bIsEnabled);
		}
	}
}