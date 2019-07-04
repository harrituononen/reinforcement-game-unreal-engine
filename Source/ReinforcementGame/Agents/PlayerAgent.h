// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Agent.h"
#include "PlayerAgent.generated.h"

/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API APlayerAgent : public AAgent
{
    GENERATED_BODY()

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void MoveAgent(float ForwardValue, float RightValue, float DeltaTime) override;
    virtual void HandleDeath() override;

private:
    static const FName  MoveForwardBinding;
    static const FName  MoveRightBinding;
    static const FName  FireBinding;
};
