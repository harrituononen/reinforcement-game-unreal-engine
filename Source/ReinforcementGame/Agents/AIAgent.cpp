// Fill out your copyright notice in the Description page of Project Settings.

#include "AIAgent.h"
#include "Components/InputComponent.h"
#include "ReinforcementGameGameMode.h"
#include "Engine/World.h"

void AAIAgent::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{}

void AAIAgent::Init(int ID, FLinearColor AgentColor, EAgentType AgentType, TArray<FVector> WallLocations, int32 InMoveSpeed)
{
    Super::Init(ID, AgentColor, AgentType, WallLocations, InMoveSpeed);
    bIsAtTarget = true;
    Shoot = 0;
    TargetX = 0.0f;
    TargetY = 0.0f;
    TargetReachedDistance = 10.0f;
    bFired = false;
    bTargetLeftSide = false;
}

void AAIAgent::Tick(float DeltaTime)
{
    bFired = false;
    if (!bLost)
    {
        if (!bIsAtTarget)
        {
            float ForwardValue = 0.0f;
            float RightValue = 0.0f;
            if (TargetX > GetActorLocation().X + TargetReachedDistance)
                ForwardValue = 1.0f;
            else if (TargetX < GetActorLocation().X - TargetReachedDistance)
                ForwardValue = -1.0f;

            if (TargetY > GetActorLocation().Y + TargetReachedDistance)
                RightValue = 1.0f;
            else if (TargetY < GetActorLocation().Y - TargetReachedDistance)
                RightValue = -1.0f;

            if (ForwardValue == 0.0f && RightValue == 0.0f)
                bIsAtTarget = true;
            else
                MoveAgent(ForwardValue, RightValue, DeltaTime);

            if (bCanFire)
            {
                Fire();
                bFired = true;
            }
        }
        else
        {
            if (bCanFire)
            {
                Fire();
                bFired = true;
            }
            if (bTargetLeftSide)
                TargetY = WallLocations[3].Y - 101.0f;
            else
                TargetY = WallLocations[4].Y + 101.0f;
            bTargetLeftSide = !bTargetLeftSide;
            if (GameMode->GetGameMode() == EGameMode::ESurviveHard)
                TargetX = GetActorLocation().X;
            else
                TargetX = FMath::RandRange(WallLocations[1].X + 101.0f, WallLocations[2].X - 101.0f);
            bIsAtTarget = false;
        }
    }
}

void AAIAgent::HandleDeath()
{
    Super::HandleDeath();
}

void AAIAgent::MoveAgent(float ForwardValue, float RightValue, float DeltaTime)
{
    Super::MoveAgent(ForwardValue, RightValue, DeltaTime);
}

void AAIAgent::ResetAgent()
{
    Super::ResetAgent();
}
