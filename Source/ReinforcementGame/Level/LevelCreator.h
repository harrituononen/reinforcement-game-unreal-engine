// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Agents/Agent.h"
#include "LevelCreator.generated.h"

UCLASS()
class REINFORCEMENTGAME_API ALevelCreator : public AActor
{
    GENERATED_BODY()

public:
    ALevelCreator();
    void InitDefaults(UWorld* InWorld, int32 InWidth, int32 InHeight, EAgentType InAgentOneType, EAgentType InAgentTwoType, int32 InNumOfAgents);
    void CreateLevelArray();
    void SetWallsToLevelEdges();
    void BuildLevel();
    TArray<FVector> CreateLevel(UWorld* InWorld, int32 InWidth, int32 InHeight, EAgentType InAgentOneType, EAgentType InAgentTwoType, int32 NumOfAgents);
    void CreateFloor();
    void CreateWalls();
    void SetCamera(FVector InCamLocation);
    void SetSpawnPoints();
    TArray<class ASpawnPoint*> GetSpawnPoints() const { return SpawnPoints; }

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    class UStaticMeshComponent*         FloorMesh;
    UPROPERTY()
    class UMaterialInterface*           FloorMaterial;
    UPROPERTY()
    class UMaterialInstanceDynamic*     FloorDynamicMaterialInstance;

    UPROPERTY()
    class UStaticMeshComponent*         WallMesh;
    UPROPERTY()
    class UMaterialInterface*           WallMaterial;
    UPROPERTY()
    class UMaterialInstanceDynamic*     WallDynamicMaterialInstance;

    TArray<bool> LevelArray;
    TArray<class ASpawnPoint*> SpawnPoints;

    UPROPERTY(EditAnywhere, Category = LevelCreator)
    int32 WallWidth;
    UPROPERTY(EditAnywhere, Category = LevelCreator)
    int32 Width;
    UPROPERTY(EditAnywhere, Category = LevelCreator)
    int32 Height;

    FVector LevelCubeDefaultExtent;
    FVector FloorExtent;

    EAgentType AgentOneType;
    EAgentType AgentTwoType;

    int32 NumOfAgents;

    UPROPERTY()
    AActor* Floor;

    UPROPERTY(EditAnywhere, Category = Camera)
    AActor* Camera;

    UPROPERTY()
    UWorld* World;

    TArray<FVector> WallLocations;
};
