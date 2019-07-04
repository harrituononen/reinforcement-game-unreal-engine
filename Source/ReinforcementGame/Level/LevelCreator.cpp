// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelCreator.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Engine/World.h"
#include "LevelCube.h"
#include "Agents/Agent.h"
#include "SpawnPoint.h"


ALevelCreator::ALevelCreator()
{
    PrimaryActorTick.bCanEverTick = false;

    Width = 15;
    Height = 15;
    World = nullptr;
}

void ALevelCreator::BeginPlay()
{}

void ALevelCreator::InitDefaults(UWorld* InWorld, int32 InWidth, int32 InHeight, EAgentType InAgentOneType, EAgentType InAgentTwoType, int32 InNumOfAgents)
{
    Width = InWidth;
    Height = InHeight;
    World = InWorld;
    AgentOneType = InAgentOneType;
    AgentTwoType = InAgentTwoType;
    NumOfAgents = InNumOfAgents;

    FVector Location(0.0f, 0.0f, 0.0f);
    FRotator Rotation(0.0f, 0.0f, 0.0f);
    FActorSpawnParameters SpawnInfo;
    if (World)
    {
        Floor = World->SpawnActor<ALevelCube>(ALevelCube::StaticClass(), Location, Rotation, SpawnInfo);
        Floor->SetActorLabel("Floor");
        FVector TempOrigin;
        Floor->GetActorBounds(false, TempOrigin, LevelCubeDefaultExtent);
    }
}

void ALevelCreator::CreateLevelArray()
{}

void ALevelCreator::SetWallsToLevelEdges()
{
    for (int i = 0; i < Width * Height; ++i)
    {
        if (i % Width == 0 && i > 0)
        {
            LevelArray[i] = true;
            LevelArray[i - 1] = true;
        }
        else if (i < Width || i >= Width * (Height - 1))
            LevelArray[i] = true;
    }
}

void ALevelCreator::BuildLevel()
{
    if (World)
    {
        CreateFloor();
        CreateWalls();
        SetCamera(FVector(Floor->GetActorLocation().X, Floor->GetActorLocation().Y, Floor->GetActorLocation().Z + Width * 110.0f));
        SetSpawnPoints();
    }
}

TArray<FVector> ALevelCreator::CreateLevel(UWorld* InWorld, int32 InWidth, int32 InHeight, EAgentType InAgentOneType, EAgentType InAgentTwoType, int32 InNumOfAgents)
{
    InitDefaults(InWorld, InWidth, InHeight, InAgentOneType, InAgentTwoType, InNumOfAgents);
    CreateLevelArray();
    BuildLevel();
    return WallLocations;
}

void ALevelCreator::CreateFloor()
{
    Floor->SetActorScale3D(FVector(Height, Width, 1));
    
    FVector Origin;
    FVector BoxExtent;
    Floor->GetActorBounds(false, Origin, BoxExtent);
    Floor->SetActorLocation(FVector(BoxExtent.X, BoxExtent.Y, -BoxExtent.Z));

#if WITH_EDITOR
    Floor->SetFolderPath(FName("Level"));
#endif
}

void ALevelCreator::CreateWalls()
{
    FActorSpawnParameters SpawnInfo;
    FRotator Rotation(0.f, 0.f, 0.f);

    FVector BottomWallLocation(LevelCubeDefaultExtent.X, LevelCubeDefaultExtent.Y * Width, LevelCubeDefaultExtent.Z);
    AActor* BottomWall = World->SpawnActor<ALevelCube>(ALevelCube::StaticClass(), BottomWallLocation, Rotation);
    BottomWall->SetActorScale3D(FVector(1, Width - 2, 1));

    FVector TopWallLocation((LevelCubeDefaultExtent.X * Height) * 2.0f - LevelCubeDefaultExtent.X, LevelCubeDefaultExtent.Y * Width, LevelCubeDefaultExtent.Z);
    AActor* TopWall = World->SpawnActor<ALevelCube>(ALevelCube::StaticClass(), TopWallLocation, Rotation);
    TopWall->SetActorScale3D(FVector(1, Width - 2, 1));

    FVector MiddleWallLocation((LevelCubeDefaultExtent.X * Height), LevelCubeDefaultExtent.Y * Width, LevelCubeDefaultExtent.Z * 0.25f);
    AActor* MiddleWall = World->SpawnActor<ALevelCube>(ALevelCube::StaticClass(), MiddleWallLocation, Rotation);
    MiddleWall->SetActorScale3D(FVector(1, Width - 2, 0.25f));

    FVector LeftWallLocation(LevelCubeDefaultExtent.X * Height, LevelCubeDefaultExtent.Y, LevelCubeDefaultExtent.Z);
    AActor* LeftWall = World->SpawnActor<ALevelCube>(ALevelCube::StaticClass(), LeftWallLocation, Rotation);
    LeftWall->SetActorScale3D(FVector(Height, 1, 1));

    FVector RightWallLocation(LevelCubeDefaultExtent.X * Height, LevelCubeDefaultExtent.Y * Width * 2.f - LevelCubeDefaultExtent.Y, LevelCubeDefaultExtent.Z);
    AActor* RightWall = World->SpawnActor<ALevelCube>(ALevelCube::StaticClass(), RightWallLocation, Rotation);
    RightWall->SetActorScale3D(FVector(Height, 1, 1));

    WallLocations.Add(TopWallLocation);
    WallLocations.Add(BottomWallLocation);
    WallLocations.Add(MiddleWallLocation);
    WallLocations.Add(RightWallLocation);
    WallLocations.Add(LeftWallLocation);

#if WITH_EDITOR
    BottomWall->SetActorLabel("BottomWall");
    BottomWall->SetFolderPath(FName("Level"));
    TopWall->SetActorLabel("TopWall");
    TopWall->SetFolderPath(FName("Level"));
    MiddleWall->SetActorLabel("MiddleWall");
    MiddleWall->SetFolderPath(FName("Level"));
    LeftWall->SetActorLabel("LeftWall");
    LeftWall->SetFolderPath(FName("Level"));
    RightWall->SetActorLabel("RightWall");
    RightWall->SetFolderPath(FName("Level"));
#endif
}

void ALevelCreator::SetCamera(FVector InCamLocation)
{
    TArray<class AActor*> FoundCameras;
    UGameplayStatics::GetAllActorsWithTag(World, FName(TEXT("Camera")), FoundCameras);
    if (FoundCameras.Num() <= 0)
    {
        UE_LOG(LogClass, Warning, TEXT("No cameras found in the scene!"));
    }
    else
    {
        for (AActor* Cam : FoundCameras)
            Camera = Cam;
        Camera->SetActorLocation(InCamLocation);
    }
}

void ALevelCreator::SetSpawnPoints()
{
    FVector Origin;
    FVector FloorExtent;
    Floor->GetActorBounds(false, Origin, FloorExtent);

    if (AgentOneType == EAgentType::EPlayer || AgentTwoType == EAgentType::EPlayer)
        UE_LOG(LogClass, Log, TEXT("WHAAAT"));

    float X = Floor->GetActorLocation().X - FloorExtent.X + LevelCubeDefaultExtent.Y * 2 + 75.0f;
    float Y = Floor->GetActorLocation().Y - FloorExtent.Y + LevelCubeDefaultExtent.Y * 2 + 75.0f;
    float Z = LevelCubeDefaultExtent.Z + 1.0f;

    FActorSpawnParameters SpawnInfo;
    ASpawnPoint* AgentOneSpawn = World->SpawnActor<ASpawnPoint>(ASpawnPoint::StaticClass(), FVector(X, Y, Z), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    AgentOneSpawn->SetID(0);
    AgentOneSpawn->SetSpawningAgentType(AgentOneType);
    SpawnPoints.Add(AgentOneSpawn);

    X = Floor->GetActorLocation().X + FloorExtent.X - LevelCubeDefaultExtent.Y * 2 - 75.0f;
    Y = Floor->GetActorLocation().Y + FloorExtent.Y - LevelCubeDefaultExtent.Y * 2 - 75.0f;

    ASpawnPoint* AgentTwoSpawn = World->SpawnActor<ASpawnPoint>(ASpawnPoint::StaticClass(), FVector(X, Y, Z), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
    AgentTwoSpawn->SetID(1);
    AgentTwoSpawn->SetSpawningAgentType(AgentTwoType);
    SpawnPoints.Add(AgentTwoSpawn);

    if (NumOfAgents >= 3)
    {
        X = Floor->GetActorLocation().X - LevelCubeDefaultExtent.Y * 2 - 75.0f;
        Y = Floor->GetActorLocation().Y + FloorExtent.Y - LevelCubeDefaultExtent.Y * 2 - 75.0f;

        ASpawnPoint* AgentThreeSpawn = World->SpawnActor<ASpawnPoint>(ASpawnPoint::StaticClass(), FVector(X, Y, Z), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo);
        AgentThreeSpawn->SetID(2);
        AgentThreeSpawn->SetSpawningAgentType(AgentOneType);
        SpawnPoints.Add(AgentThreeSpawn);
    }


#if WITH_EDITOR
    for (ASpawnPoint* SpawnPoint : SpawnPoints)
    {
        SpawnPoint->SetActorLabel("Spawnpoint ID: " + FString::FromInt(SpawnPoint->GetID()));
        SpawnPoint->SetFolderPath(FName(TEXT("Level")));
    }
#endif
}

