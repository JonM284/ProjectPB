// Fill out your copyright notice in the Description page of Project Settings.


#include "PinBrawlersPlayerController.h"

void APinBrawlersPlayerController::SetPlayerEnableState(bool bPlayerEnabled)
{
    if(bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }else{
        GetPawn()->DisableInput(this);
    }

}