// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AgentPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API AAgentPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    void AgentPlayerController();
    virtual void Possess(class APawn* InPawn) override;

};
