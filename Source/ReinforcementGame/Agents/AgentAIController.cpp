// Fill out your copyright notice in the Description page of Project Settings.

#include "AgentAIController.h"
#include "Agent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NeuralNetworkAgent.h"

AAgentAIController::AAgentAIController(const class FObjectInitializer& ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    BehaviorComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
    BlackboardComponent = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
}

void AAgentAIController::Tick(float DeltaTime)
{}

void AAgentAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AAgentAIController::Possess(APawn * InPawn)
{
    Super::Possess(InPawn);
}

void AAgentAIController::UnPossess()
{}
