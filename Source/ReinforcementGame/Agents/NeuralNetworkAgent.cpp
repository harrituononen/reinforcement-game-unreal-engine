// Fill out your copyright notice in the Description page of Project Settings.

#include "NeuralNetworkAgent.h"
#include "ReinforcementGameGameMode.h"
#include "FGameStateStruct.h"
#include "FNetworkDataStruct.h"
#include "Projectile.h"
#include "Agents/AIAgent.h"
#include "Agents/FFloatArrayStruct.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "FProjectileLocationsStruct.h"
#include "Engine/World.h"

void ANeuralNetworkAgent::Init(int ID, FLinearColor AgentColor, EAgentType AgentType, TArray<FVector> WallLocations, int32 InMoveSpeed
                                , float InContinueReward, float InWinReward, float InLoseReward, float InMaxLevelHeight, float InMinLevelHeight)
{
    Super::Init(ID, AgentColor, AgentType, WallLocations, InMoveSpeed);
    ContinueReward = InContinueReward;
    WinReward = InWinReward;
    LoseReward = InLoseReward;
    MaxLevelHeight = InMaxLevelHeight;
    MinLevelHeight = InMinLevelHeight;
    ResetAgent();
    InitMaterialAndColor();
    EpisodeLength = 0;
    BulletsInPlay = 0;
    ResetCounter = 300;
    bHitFront = false;
    bHitBehind = false;
    bHitLeft = false;
    bHitRight = false;
    HighestEpisodeLength = 0;
    HittingShot = FVector2D(0.0f, 0.0f);
    Won = 0;
    Lost = 0;
    GameLengthInSeconds = 0;
    EpisodeLengthInSeconds = 0;
    HighestEpisodeLengthInSeconds = 0;
    EpisodeMinutes = 0;
    EpisodeSeconds = 0;
    Minutes = 0;
    Seconds = 0;
    AverageMinutes = 0;
    AverageSeconds = 0;
}

void ANeuralNetworkAgent::Tick(float DeltaTime)
{
    HandleInputs(DeltaTime);
}

void ANeuralNetworkAgent::BeginPlay()
{
    Super::BeginPlay();
}

void ANeuralNetworkAgent::HandleDeath()
{
    Super::HandleDeath();
}

void ANeuralNetworkAgent::MoveAgent(float ForwardValue, float RightValue, float DeltaTime)
{
    Super::MoveAgent(ForwardValue, RightValue, DeltaTime);
}

void ANeuralNetworkAgent::Fire()
{
    Super::Fire();
}

void ANeuralNetworkAgent::ResetAgent()
{
    Super::ResetAgent();
    bNewInputsReceived = false;
    HittingShot = FVector2D(0.0f, 0.0f);
}

void ANeuralNetworkAgent::HandleInputs(float DeltaTime)
{
    GameLengthInSeconds += DeltaTime;
    EpisodeLengthInSeconds += DeltaTime;
    Minutes = (int)GameLengthInSeconds / 60;
    Seconds = (int)GameLengthInSeconds % 60;
    GameMode->SetGameLength(GameLengthInSeconds);
    EpisodeMinutes = (int)EpisodeLengthInSeconds / 60;
    EpisodeSeconds = (int)EpisodeLengthInSeconds % 60;
    GameMode->SetEpisodeLengthString(EpisodeMinutes, EpisodeSeconds);
    if (bNewInputsReceived && GameMode->GetPlayState() == EPlayState::EPlay)
    {
        bNewInputsReceived = false;
        float ForwardValue = 0.0f;
        float RightValue = 0.0f;
        float Reward = 0.0f;
        bool bIsGameDone = false;
        bool bHit = false;

        if (Action == 0) // Left
            RightValue = -1.0f;
        else if (Action == 1) // Right
            RightValue = 1.0f;
        else if (Action == 2) // Up
            ForwardValue = 1.0f;
        else if (Action == 3) // Down
            ForwardValue = -1.0f;
        else if (Action == 4 && !bLost && GameMode->GetGameMode() == EGameMode::EDestroy) // Fire
            Fire();

        if ((RightValue != 0.0f || ForwardValue != 0.0f) && !bLost)
            MoveAgent(ForwardValue, RightValue, DeltaTime);

        Reward = GameMode->CheckGameStatus(ID, bLost);
        if (Reward != ContinueReward)
        {


            bIsGameDone = true;
            GameMode->SetPlayState(EPlayState::EWait);
            if (EpisodeLengthInSeconds > HighestEpisodeLengthInSeconds)
            {
                HighestEpisodeLengthInSeconds = EpisodeLengthInSeconds;
                GameMode->SetMaxEpisodeLengthString(EpisodeMinutes, EpisodeSeconds);
            }
            if (EpisodeLength > HighestEpisodeLength)
                HighestEpisodeLength = EpisodeLength;
            EpisodeLengthsInSeconds.Add(EpisodeLengthInSeconds);
            if (EpisodeLengthsInSeconds.Num() > 100)
                EpisodeLengthsInSeconds.RemoveAt(0);

            int32 SecondsSum = 0;
            for (int32 GameLength : EpisodeLengthsInSeconds)
                SecondsSum += GameLength;
            float AverageSeconds = float(SecondsSum) / EpisodeLengthsInSeconds.Num();
            AverageMinutes = (int)AverageSeconds / 60;
            AverageSeconds = (int)AverageSeconds % 60;
            GameMode->SetAverageEpisodeLengthString(AverageMinutes, AverageSeconds);

            EpisodeLengths.Add(EpisodeLength);
            if (EpisodeLengths.Num() > 100)
                EpisodeLengths.RemoveAt(0);
            int32 Sum = 0;
            for (int32 GameLength : EpisodeLengths)
                Sum += GameLength;
            float Average = float(Sum) / EpisodeLengths.Num();

            if (GameMode->GetGameMode() == EGameMode::EDestroy)
            {
                if (Reward == WinReward)
                    Won += 1;
                else
                    Lost += 1;
                UE_LOG(LogClass, Log, TEXT("---------------------"));
                UE_LOG(LogClass, Log, TEXT("Won - Lost %d - %d"), Won, Lost);
            }
            UE_LOG(LogClass, Log, TEXT("AVG game length: %f Highest: %d"), Average, HighestEpisodeLength);
            EpisodeLength = 0;
            EpisodeLengthInSeconds = 0;
        }
        else
        {
            EpisodeLength += 1;
            if (EpisodeLength >= ResetCounter && GameMode->GetGameMode() == EGameMode::EDestroy)
            {
                bIsGameDone = true;
                GameMode->SetPlayState(EPlayState::EWait);
                EpisodeLength = 0;
                Reward += LoseReward;
            }
        }

        State = GetState();
        FNetworkDataStruct NetworkData;
        NetworkData.Set(OldState, Action, Reward, State, bIsGameDone, HighestEpisodeLength);
        OldState = State;
        SendNetworkData(NetworkData);
    }
}

void ANeuralNetworkAgent::ChangeBulletsInPlay(int Change)
{
    BulletsInPlay += Change;
}

void ANeuralNetworkAgent::SetOpponent(AAgent * InOpponent)
{
    Enemy = InOpponent;
}

void ANeuralNetworkAgent::SetHittingShot(FVector2D InHittingShot)
{
    HittingShot = InHittingShot;
}

void ANeuralNetworkAgent::OnTakeAStepEvent(int Action)
{
    this->Action = Action;
    bNewInputsReceived = true;
}

FGameStateStruct ANeuralNetworkAgent::GetState()
{
    if (World)
    {
        // Agent and enemy locations
        FVector2D MyLocation = FVector2D(GetActorLocation().X, GetActorLocation().Y);
        FVector2D EnemyLocation = FVector2D(Enemy->GetActorLocation().X, Enemy->GetActorLocation().Y);
        float MyX = MyLocation.X;
        float MyY = MyLocation.Y;
        float EnemyX = EnemyLocation.X;
        float EnemyY = EnemyLocation.Y;

        // The X-axis distance to enemy normalized
        float DistanceToEnemyX = 0.0f;
        if (MyX >= EnemyX)
            DistanceToEnemyX = MyX - EnemyX;
        else
            DistanceToEnemyX = EnemyX - MyX;
        float NormalizedDistanceToEnemyX = UKismetMathLibrary::NormalizeToRange(DistanceToEnemyX, MinLevelHeight, MaxLevelHeight);

        // Is the agent too far away to hit the enemy?
        bool bTooFarAway = false;


        // Is the agent next to a wall?
        TArray<FVector> WallLocations = GameMode->GetWallLocations();
        bool bCloseToTopWall = false;
        bool bCloseToBottomWall = false;
        bool bCloseToMiddleWall = false;
        bool bCloseToRightWall = false;
        bool bCloseToLeftWall = false;

        // Is the enemy left or right of the agent?
        bool bEnemyLeft = false;
        bool bEnemyRight = false;

        // Agent velocity
        float XVelocity = GetXVelocity();
        float YVelocity = GetYVelocity();

        // Agent movement directions
        bool bMovingUp = false;
        bool bMovingDown = false;
        bool bMovingLeft = false;
        bool bMovingRight = false;

        // Enemy velocity
        float EnemyXVelocity = 0.0f;
        float EnemyYVelocity = 0.0f;
        EnemyXVelocity = Enemy->GetXVelocity();
        EnemyYVelocity = Enemy->GetYVelocity();

        // Enemy movement directions
        bool bEnemyMovingUp = false;
        bool bEnemyMovingDown = false;
        bool bEnemyMovingLeft = false;
        bool bEnemyMovingRight = false;

        // Is a bullet moving towards the enemy and is a bullet near agent or enemy?
        bool bBulletHeadingTowardsEnemy = false;
        bool bBulletInFront = false;
        bool bBulletBehind = false;
        bool bBulletLeft = false;
        bool bBulletRight = false;
        bool bBulletInFrontOfEnemy = false;
        bool bBulletBehindEnemy = false;
        bool bBulletLeftOfEnemy = false;
        bool bBulletRightOfEnemy = false;
        float AgentExtent = 50.0f;

        bool bEnemyFired = false;

        FVector2D ClosestProjectileLocation = EnemyLocation;
        FVector2D SecondClosestProjectileLocation = EnemyLocation;
        FVector2D ThirdClosestProjectileLocation = EnemyLocation;

        FVector2D ClosestProjectileLocationToEnemy = MyLocation;
        float ShortestDistanceToEnemy = 99999.0f;

        float ShortestDistanceToAgent = 99999.0f;
        float SecondShortestDistanceToAgent = 99999.0f;
        float ThirdShortestDistanceToAgent = 99999.0f;
        int32 ShotBulletsInPlay = 0;

        TArray<AActor*> FProjectiles;
        TArray<AActor*> FoundProjectiles;
        if (World)
        {
            UGameplayStatics::GetAllActorsOfClass(World, AProjectile::StaticClass(), FoundProjectiles);
            for (AActor* Actor : FoundProjectiles)
            {
                ShotBulletsInPlay += 1;
                if (Cast<AProjectile>(Actor))
                {
                    AProjectile* Projectile = Cast<AProjectile>(Actor);
                    if (Projectile->GetID() != ID)
                    {
                        float DistanceToTarget = (GetActorLocation() - Actor->GetActorLocation()).Size();
                        if (DistanceToTarget < ShortestDistanceToAgent)
                        {
                            ThirdShortestDistanceToAgent = SecondShortestDistanceToAgent;
                            ThirdClosestProjectileLocation = SecondClosestProjectileLocation;
                            SecondShortestDistanceToAgent = ShortestDistanceToAgent;
                            SecondClosestProjectileLocation = ClosestProjectileLocation;
                            ShortestDistanceToAgent = DistanceToTarget;
                            ClosestProjectileLocation = FVector2D(Actor->GetActorLocation().X, Actor->GetActorLocation().Y);
                        }
                        else if (DistanceToTarget < SecondShortestDistanceToAgent)
                        {
                            ThirdShortestDistanceToAgent = SecondShortestDistanceToAgent;
                            ThirdClosestProjectileLocation = SecondClosestProjectileLocation;
                            SecondShortestDistanceToAgent = DistanceToTarget;
                            SecondClosestProjectileLocation = FVector2D(Actor->GetActorLocation().X, Actor->GetActorLocation().Y);
                        }
                        else if (DistanceToTarget < ThirdShortestDistanceToAgent)
                        {
                            ThirdShortestDistanceToAgent = DistanceToTarget;
                            ThirdClosestProjectileLocation = FVector2D(Actor->GetActorLocation().X, Actor->GetActorLocation().Y);
                        }
                    }
                    else
                    {
                        float DistanceToEnemy = (Enemy->GetActorLocation() - Actor->GetActorLocation()).Size();
                        if (DistanceToEnemy <= ShortestDistanceToEnemy)
                        {
                            ShortestDistanceToEnemy = DistanceToEnemy;
                            ClosestProjectileLocationToEnemy = FVector2D(Actor->GetActorLocation().X, Actor->GetActorLocation().Y);
                        }
                    }
                }
            }
        }

        State.Set(MyX, MyY, EnemyX, EnemyY, MyLocation, EnemyLocation, ClosestProjectileLocation, SecondClosestProjectileLocation
                , ThirdClosestProjectileLocation, ClosestProjectileLocationToEnemy, LastShot, HittingShot, EnemyXVelocity, EnemyYVelocity
                , NormalizedDistanceToEnemyX, ShotBulletsInPlay, bBulletHeadingTowardsEnemy
                , bBulletInFront, bBulletBehind, bBulletLeft, bBulletRight, bBulletInFrontOfEnemy, bBulletBehindEnemy, bBulletLeftOfEnemy
                , bBulletRightOfEnemy
                , bMovingUp, bMovingDown, bMovingLeft, bMovingRight
                , bEnemyMovingUp, bEnemyMovingDown, bEnemyMovingLeft, bEnemyMovingRight, bTooFarAway, bEnemyLeft, bEnemyRight, bEnemyFired
                , bHitFront, bHitBehind, bHitLeft, bHitRight
                , bCloseToTopWall, bCloseToBottomWall, bCloseToMiddleWall, bCloseToRightWall, bCloseToLeftWall, bCanFire);
    }
    return State;
}

void ANeuralNetworkAgent::SetOldState(FGameStateStruct InOldState)
{
    OldState = InOldState;
}
