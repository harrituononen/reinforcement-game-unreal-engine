// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Agents/Agent.h"
#include "AIAgent.generated.h"

/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API AAIAgent : public AAgent
{
    GENERATED_BODY()
public:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    virtual void Init(int ID, FLinearColor AgentColor, EAgentType AgentType, TArray<FVector> WallLocations, int32 InMoveSpeed) override;
    virtual void Tick(float DeltaTime) override;
    virtual void HandleDeath() override;
    virtual void MoveAgent(float ForwardValue, float RightValue, float DeltaTime) override;
    virtual void ResetAgent() override;
    bool GetFired() const { return bFired; }

private:
    int32 Shoot;
    float TargetX;
    float TargetY;
    float TargetReachedDistance;
    bool bIsAtTarget;
    bool bFired;
    bool bTargetLeftSide;
};
