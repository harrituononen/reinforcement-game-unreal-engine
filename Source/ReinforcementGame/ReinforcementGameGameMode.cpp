// Fill out your copyright notice in the Description page of Project Settings.

#include "ReinforcementGameGameMode.h"
#include "Agents/Agent.h"
#include "Agents/AIAgent.h"
#include "Agents/PlayerAgent.h"
#include "Agents/NeuralNetworkAgent.h"
#include "Kismet/GameplayStatics.h"
#include "Level/SpawnPoint.h"
#include "Level/LevelCreator.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Agents/AgentAIController.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Agents/AgentPlayerController.h"

AReinforcementGameGameMode::AReinforcementGameGameMode()
{
    DefaultPawnClass = NULL;
    PlayerControllerClass = AAgentPlayerController::StaticClass();

    static ConstructorHelpers::FObjectFinder<UClass>NeuralNetworkAgentBPFinder(TEXT("Blueprint'/Game/BP/NeuralNetworkAgent_BP.NeuralNetworkAgent_BP_C'"));
    if (NeuralNetworkAgentBPFinder.Object != NULL)
        NeuralNetworkAgent_BP = NeuralNetworkAgentBPFinder.Object;

    AgentOneType = EAgentType::EAI;
    AgentTwoType = EAgentType::ENeuralNetwork;
    
    LevelWidth = 13;
    LevelHeight = 13;
    SetPlayState(EPlayState::EWait);
    GameMode = EGameMode::ESurviveHard;
    
    World = GetWorld();
    if (!World)
        UE_LOG(LogClass, Warning, TEXT("Warning: GetWorld() failed! %s"), *this->GetName());

    Episodes = 50000;
    CurrentEpisode = 0;

    if (GameMode == EGameMode::EDestroy)
    {
        ContinueReward = -1.0f;
        WinReward = 10.0f;
        LoseReward = -10.0f;
    }
    else
    {
        ContinueReward = 1.0f;
        WinReward = 100.0f;
        LoseReward = -100.0f;
    }
    PlayerMoveSpeed = 1000;
    NeuralMoveSpeed = 1000;
    AIMoveSpeed = 1000;

    ResetDelay = 0.00001f;
    AgentExtent = 100.0f;
    EpisodesString = "0";
}

void AReinforcementGameGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundSpawnPoints;
    if (World)
    {
        FActorSpawnParameters SpawnInfo;
        Level = Cast<ALevelCreator>(World->SpawnActor<ALevelCreator>(ALevelCreator::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnInfo));
        if (Level)
        {
            int32 NumOfAgents = 2;
            if (GameMode == EGameMode::ESurviveHard)
                NumOfAgents = 3;
            WallLocations = Level->CreateLevel(World, LevelWidth, LevelHeight, AgentOneType, AgentTwoType, NumOfAgents);
            MaxLevelHeight = WallLocations[0].X - 100.0f;
            MinLevelHeight = WallLocations[1].X + 100.0f;
            for (ASpawnPoint* SpawnPoint : Level->GetSpawnPoints())
                SpawnAgent(SpawnPoint->GetActorRotation(), SpawnPoint->GetActorLocation(), SpawnPoint->GetID(), SpawnPoint->GetSpawningAgentType(), World);
            for (AAgent* Agent : Agents)
            {
                if (Cast<ANeuralNetworkAgent>(Agent))
                {
                    ANeuralNetworkAgent* NNAgent = Cast<ANeuralNetworkAgent>(Agent);
                    for (AAgent* Opponent : Agents)
                        if (Opponent->GetID() != NNAgent->GetID())
                            NNAgent->SetOpponent(Opponent);
                }
            }

            SetPlayState(EPlayState::EPlay);
#if WITH_EDITOR
            for (AAgent* Agent : Agents)
                Agent->SetFolderPath(FName(TEXT("Agents")));
#endif
        }
    }
}

void AReinforcementGameGameMode::SetAIControllerClass(AAgent * Agent, UClass * AIController)
{
    Agent->AIControllerClass = AIController;
    Agent->SpawnDefaultController();
}

void AReinforcementGameGameMode::HandleAgentController(AAgent* Agent, UWorld* World)
{
    switch (Agent->GetAgentType())
    {
    case EAgentType::EPlayer:
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
        if (PlayerController)
            PlayerController->Possess(Agent);
    }
    break;
    case EAgentType::EAI:
    {
        SetAIControllerClass(Agent, AAgentAIController::StaticClass());
    }
    break;
    case EAgentType::ENeuralNetwork:
    {
        SetAIControllerClass(Agent, AAgentAIController::StaticClass());
    }
    break;
    default:
        UE_LOG(LogClass, Warning, TEXT("No suitable AgentType found."))
            break;
    }
}

void AReinforcementGameGameMode::SpawnAgent(FRotator SpawnPointRotation, FVector SpawnPointLocation, int SpawningAgentID, EAgentType SpawningAgentType, UWorld * World)
{
    FTransform AgentSpawnTransform(SpawnPointRotation, SpawnPointLocation);
    switch (SpawningAgentType)
    {
    case EAgentType::EPlayer:
    {
        APlayerAgent* Agent = Cast<APlayerAgent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this
            , APlayerAgent::StaticClass()
            , AgentSpawnTransform));
        if (Agent != nullptr)
        {
            Agent->Init(SpawningAgentID, FLinearColor::Blue, EAgentType::EPlayer, WallLocations, PlayerMoveSpeed);
            UGameplayStatics::FinishSpawningActor(Agent, AgentSpawnTransform);
            HandleAgentController(Agent, World);
            Agents.Add(Agent);
        }
    }
    break;
    case EAgentType::EAI:
    {
        AAIAgent* Agent = Cast<AAIAgent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this
            , AAIAgent::StaticClass()
            , AgentSpawnTransform));
        if (Agent != nullptr)
        {
            Agent->Init(SpawningAgentID, FLinearColor::Red, EAgentType::EAI, WallLocations, AIMoveSpeed);
            UGameplayStatics::FinishSpawningActor(Agent, AgentSpawnTransform);
            HandleAgentController(Agent, World);
            Agents.Add(Agent);
        }
        if (GameMode == EGameMode::ESurviveHard)
            AIMoveSpeed = AIMoveSpeed * 0.7f;
    }
    break;
    case EAgentType::ENeuralNetwork:
    {
        ANeuralNetworkAgent* Agent = Cast<ANeuralNetworkAgent>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this
            , NeuralNetworkAgent_BP
            , AgentSpawnTransform));
        if (Agent != nullptr)
        {
            Agent->Init(SpawningAgentID, FLinearColor(1.0f, 0.1f, 0.0f), EAgentType::ENeuralNetwork, WallLocations, NeuralMoveSpeed
                        , ContinueReward, WinReward, LoseReward, MaxLevelHeight, MinLevelHeight);
            UGameplayStatics::FinishSpawningActor(Agent, AgentSpawnTransform);
            HandleAgentController(Agent, World);
            Agents.Add(Agent);
        }
    }
    break;
    default:
    {
        UE_LOG(LogClass, Warning, TEXT("No suitable AgentType found."));
    }
    break;
    }
}

void AReinforcementGameGameMode::ResetGame()
{
    TArray<AActor*> FoundProjectiles;
    if (World)
    {
        UGameplayStatics::GetAllActorsOfClass(World, AProjectile::StaticClass(), FoundProjectiles);
        for (auto Projectile : FoundProjectiles)
            Projectile->Destroy();
    }
    for (AAgent* Agent : Agents)
    {
        Agent->ResetActor();
        Agent->ResetAgent();
    }

    CurrentEpisode += 1;
    EpisodesString = FString::FromInt(CurrentEpisode);
    if (CurrentEpisode < Episodes)
    {
        if(CurrentEpisode % 10 == 0)
            UE_LOG(LogClass, Log, TEXT("Episode %d"), CurrentEpisode);
        World->GetTimerManager().SetTimer(TimerHandle_ContinueGame, this, &AReinforcementGameGameMode::ContinueGame, ResetDelay);
    }
    else
        UE_LOG(LogClass, Log, TEXT("Game Over!"));
}

void AReinforcementGameGameMode::ContinueGame()
{
    SetPlayState(EPlayState::EPlay);
    for (AAgent* Agent : Agents)
    {
        if (Cast<ANeuralNetworkAgent>(Agent))
        {
            ANeuralNetworkAgent* NNAgent = Cast<ANeuralNetworkAgent>(Agent);
            NNAgent->StartNeuralNetwork();
        }
    }
}

void AReinforcementGameGameMode::SetPlayState(EPlayState InPlayState)
{
    CurrentPlayState = InPlayState;
}

void AReinforcementGameGameMode::EndMatch(int32 AgentID)
{
    SetPlayState(EPlayState::EWait);

    CurrentEpisode += 1;
    UE_LOG(LogClass, Log, TEXT("Current Episode %d"), CurrentEpisode);
    ResetGame();
    if (CurrentEpisode < Episodes)
        SetPlayState(EPlayState::EPlay);
    else
        UE_LOG(LogClass, Log, TEXT("Game Over!"));
}

float AReinforcementGameGameMode::CheckGameStatus(int AgentID, bool bAgentLost)
{
    float Reward = ContinueReward;

    if (bAgentLost)
    {
        //UE_LOG(LogClass, Warning, TEXT("You lost!"))
        Reward = LoseReward;
    }

    for (AAgent* Agent : Agents)
    {
        if (Agent->GetID() != AgentID && Agent->GetLost())
        {
            if (bAgentLost)
                UE_LOG(LogClass, Warning, TEXT("Both lost!"))
            Reward = WinReward;
        }
        else if (Agent->GetID() != AgentID && !Agent->GetLost() && !bAgentLost)
            Reward = ContinueReward;
    }

    if (Reward != ContinueReward)
        SetPlayState(EPlayState::EWait);

    return Reward;
}

int AReinforcementGameGameMode::GetBulletsInPlay(int AgentID)
{
    int32 Bullets = 0;
    TArray<AActor*> FoundProjectiles;
    UGameplayStatics::GetAllActorsOfClass(World, AProjectile::StaticClass(), FoundProjectiles);
    for (AActor* Actor : FoundProjectiles)
        if (Cast<AProjectile>(Actor))
        {
            AProjectile* Projectile = Cast<AProjectile>(Actor);
            if (Projectile->GetID() == AgentID)
                Bullets += 1;
        }
    return Bullets;
}

void AReinforcementGameGameMode::AddToProjectiles(AProjectile * Projectile)
{
    Projectiles.Add(Projectile);
}

void AReinforcementGameGameMode::RemoveFromProjectiles(AProjectile * Projectile)
{
    Projectiles.Remove(Projectile);
}

void AReinforcementGameGameMode::SetGameLengthString(int32 Minutes, int32 Seconds)
{
    FString Min = FString::FromInt(Minutes);
    if (Minutes < 10)
        Min = "0" + Min;
    FString Sec = FString::FromInt(Seconds);
    if (Seconds < 10)
        Sec = "0" + Sec;
    GameLengthString = Min + ":" + Sec;
}

void AReinforcementGameGameMode::SetMaxEpisodeLengthString(int32 Minutes, int32 Seconds)
{
    FString Min = FString::FromInt(Minutes);
    if (Minutes < 10)
        Min = "0" + Min;
    FString Sec = FString::FromInt(Seconds);
    if (Seconds < 10)
        Sec = "0" + Sec;
    MaxEpisodeLengthString = Min + ":" + Sec;
}

void AReinforcementGameGameMode::SetAverageEpisodeLengthString(int32 Minutes, int32 Seconds)
{
    FString Min = FString::FromInt(Minutes);
    if (Minutes < 10)
        Min = "0" + Min;
    FString Sec = FString::FromInt(Seconds);
    if (Seconds < 10)
        Sec = "0" + Sec;
    AverageEpisodeLengthString = Min + ":" + Sec;
}

void AReinforcementGameGameMode::SetEpisodeLengthString(int32 Minutes, int32 Seconds)
{
    FString Min = FString::FromInt(Minutes);
    if (Minutes < 10)
        Min = "0" + Min;
    FString Sec = FString::FromInt(Seconds);
    if (Seconds < 10)
        Sec = "0" + Sec;
    EpisodeLengthString = Min + ":" + Sec;
}

void AReinforcementGameGameMode::SetGameLength(float Seconds)
{
    GameLength = Seconds;
    int32 _Seconds = (int)Seconds % 60;
    int32 Minutes = (int)Seconds / 60;
    int32 Hours = (int)Seconds / 3600;
    Minutes = Minutes - Hours * 60;
    FString Min = FString::FromInt(Minutes);
    if (Minutes < 10)
        Min = "0" + Min;
    FString Sec = FString::FromInt(_Seconds);
    if (_Seconds < 10)
        Sec = "0" + Sec;
    FString H = FString::FromInt(Hours);
    if (Hours < 10)
        H = "0" + H;
    GameLengthString = H + ":" + Min + ":" + Sec;
}
