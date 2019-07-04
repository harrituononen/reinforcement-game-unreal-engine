// Fill out your copyright notice in the Description page of Project Settings.

#include "AgentPlayerController.h"

void AAgentPlayerController::AgentPlayerController()
{
    bAutoManageActiveCameraTarget = false;
}

void AAgentPlayerController::Possess(APawn * InPawn)
{
    Super::Possess(InPawn);
}
