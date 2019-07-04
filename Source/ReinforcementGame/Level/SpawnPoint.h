// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Agents/Agent.h"
#include "SpawnPoint.generated.h"

/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API ASpawnPoint : public ATargetPoint
{
    GENERATED_BODY()

public:
    int GetID() const { return ID; }
    EAgentType GetSpawningAgentType() const { return SpawningAgentType; }
    void SetSpawningAgentType(EAgentType NewSpawningAgentType);
    void SetID(int NewID);
private:
    UPROPERTY(EditAnywhere, Category = Spawning)
    int ID;

    UPROPERTY(EditAnywhere, Category = Spawning)
    EAgentType SpawningAgentType;
};