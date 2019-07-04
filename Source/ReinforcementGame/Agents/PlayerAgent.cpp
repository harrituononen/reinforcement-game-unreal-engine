// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAgent.h"
#include "Components/InputComponent.h"
#include "ReinforcementGameGameMode.h"
#include "Engine/World.h"

const FName APlayerAgent::MoveForwardBinding("MoveForward");
const FName APlayerAgent::MoveRightBinding("MoveRight");
const FName APlayerAgent::FireBinding("Fire");

void APlayerAgent::Tick(float DeltaTime)
{
    if (GameMode->GetPlayState() == EPlayState::EPlay)
    {
        const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
        const float RightValue = GetInputAxisValue(MoveRightBinding);

        //MoveAgent(ForwardValue, RightValue, DeltaTime);
        MoveAgentAStep(ForwardValue, RightValue, DeltaTime);
    }
}

void APlayerAgent::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(MoveForwardBinding);
    PlayerInputComponent->BindAxis(MoveRightBinding);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AAgent::Fire);
}

void APlayerAgent::MoveAgent(float ForwardValue, float RightValue, float DeltaTime)
{
    Super::MoveAgent(ForwardValue, RightValue, DeltaTime);
}

void APlayerAgent::HandleDeath()
{
    Super::HandleDeath();
}
