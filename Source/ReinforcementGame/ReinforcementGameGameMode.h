// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Agents/Agent.h"
#include "Level/LevelCreator.h"
#include "ReinforcementGameGameMode.generated.h"
class AProjectile;
enum class EPlayState : uint8
{
    EPlay,
    EWait,
};
enum class EGameMode : uint8
{
    ESurvive,
    ESurviveHard,
    EDestroy
};
/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API AReinforcementGameGameMode : public AGameMode
{
    GENERATED_BODY()

public:
    AReinforcementGameGameMode();

    virtual void BeginPlay() override;

    void SetAIControllerClass(class AAgent* Agent, class UClass* AIController);
    void HandleAgentController(AAgent* Agent, UWorld* World);
    void SpawnAgent(FRotator SpawnPointRotation, FVector SpawnPointLocation, int SpawningAgentID, EAgentType SpawningAgentType, UWorld* World);
    UFUNCTION(BlueprintCallable)
    void ResetGame();
    void ContinueGame();
    void SetPlayState(EPlayState InPlayState);
    EPlayState GetPlayState() const { return CurrentPlayState; }
    void EndMatch(int32 AgentID);
    FORCEINLINE class TArray<class AAgent*> GetAgents() { return Agents; }
    FORCEINLINE class TArray<FVector> GetWallLocations() { return WallLocations; }
    float CheckGameStatus(int AgentID, bool bAgentLost);
    int GetBulletsInPlay(int AgentID);
    TArray<AProjectile*> GetProjectiles() { return Projectiles; }
    void AddToProjectiles(AProjectile* Projectile);
    void RemoveFromProjectiles(AProjectile* Projectile);
    EGameMode GetGameMode() { return GameMode; }
    void SetGameLengthString(int32 Minutes, int32 Seconds);
    UFUNCTION(BlueprintCallable)
    FString GetGameLengthString() const { return GameLengthString; }
    void SetMaxEpisodeLengthString(int32 Minutes, int32 Seconds);
    UFUNCTION(BlueprintCallable)
    FString GetMaxEpisodeLengthString() const { return MaxEpisodeLengthString; }
    void SetAverageEpisodeLengthString(int32 Minutes, int32 Seconds);
    UFUNCTION(BlueprintCallable)
    FString GetAverageEpisodeLengthString() const { return AverageEpisodeLengthString; }
    void SetEpisodeLengthString(int32 Minutes, int32 Seconds);
    UFUNCTION(BlueprintCallable)
    FString GetEpisodeLengthString() const { return EpisodeLengthString; }
    UFUNCTION(BlueprintCallable)
    FString GetEpisodesString() const { return EpisodesString; }

    void SetGameLength(float Seconds);
    UFUNCTION(BlueprintCallable)
    float GetGameLength() const { return GameLength; }

    UPROPERTY(BlueprintReadWrite)
    TSubclassOf<class ANeuralNetworkAgent> NeuralNetworkAgent_BP;

private:
    UWorld* World;
    UPROPERTY()
    TArray<class ASpawnPoint*> SpawnPoints;
    TArray<AProjectile*> Projectiles;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TArray<class AAgent*> Agents;

    UPROPERTY()
    ALevelCreator*  Level;
    int32           LevelWidth;
    int32           LevelHeight;
    float           MaxLevelHeight;
    float           MinLevelHeight;

    EAgentType AgentOneType;
    EAgentType AgentTwoType;

    EPlayState CurrentPlayState;
    EGameMode GameMode;

    int32 Episodes;
    int32 CurrentEpisode;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    TArray<FVector> WallLocations;
    int AgentExtent;

    float LoseReward;
    float WinReward;
    float ContinueReward;

    float ResetDelay;

    int32 PlayerMoveSpeed;
    int32 NeuralMoveSpeed;
    int32 AIMoveSpeed;

    FTimerHandle TimerHandle_ContinueGame;

    FString MaxEpisodeLengthString;
    FString GameLengthString;
    FString AverageEpisodeLengthString;
    FString EpisodeLengthString;
    FString EpisodesString;

    float GameLength;
    int32 Hours;
    int32 Minutes;
    int32 Seconds;

};
