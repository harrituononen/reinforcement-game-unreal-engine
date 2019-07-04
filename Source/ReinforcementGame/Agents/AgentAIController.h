// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "AgentAIController.generated.h"

class UTensorflowComponent;
/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API AAgentAIController : public AAIController
{
    GENERATED_BODY()

public:
    AAgentAIController(const class FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;

    virtual void BeginPlay() override;
    virtual void Possess(class APawn* InPawn) override;
    virtual void UnPossess() override;

    FORCEINLINE class UBehaviorTreeComponent* GetBehaviorComponent() const { return BehaviorComponent;  }
    FORCEINLINE class UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent;  }

private:
    UPROPERTY()
    UBehaviorTreeComponent* BehaviorComponent;
    UPROPERTY()
    UBlackboardComponent* BlackboardComponent;

};
