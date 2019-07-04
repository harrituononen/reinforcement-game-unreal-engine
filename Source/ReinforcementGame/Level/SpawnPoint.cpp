// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnPoint.h"

void ASpawnPoint::SetSpawningAgentType(EAgentType NewSpawningAgentType)
{
    SpawningAgentType = NewSpawningAgentType;
}

void ASpawnPoint::SetID(int NewID)
{
    ID = NewID;
}
