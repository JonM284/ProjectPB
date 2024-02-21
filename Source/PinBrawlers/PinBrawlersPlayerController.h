// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PinBrawlersPlayerController.generated.h"

UCLASS()
class PINBRAWLERS_API APinBrawlersPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void SetPlayerEnableState(bool bPlayerEnabled);

};
