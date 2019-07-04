// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Agents/Agent.h"
#include "Agents/FFloatArrayStruct.h"
#include "FGameStateStruct.h"
#include "FNetworkDataStruct.h"
#include "NeuralNetworkAgent.generated.h"
/**
 */
UCLASS()
class REINFORCEMENTGAME_API ANeuralNetworkAgent : public AAgent
{
    GENERATED_BODY()

public:
    void Init(int ID, FLinearColor AgentColor, EAgentType AgentType, TArray<FVector> WallLocations, int32 InMoveSpeed, float InContinueReward, float InWinReward, float InLoseReward
                , float InMaxLevelHeight, float InMinLevelHeight);
    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;
    virtual void HandleDeath() override;
    virtual void MoveAgent(float ForwardValue, float RightValue, float DeltaTime) override;
    virtual void Fire() override;
    virtual void ResetAgent() override;
    void HandleInputs(float DeltaTime);
    void ChangeBulletsInPlay(int Change);
    void SetOpponent(AAgent* InOpponent);
    void SetHittingShot(FVector2D InHittingShot);

    UFUNCTION(BlueprintCallable)
    void OnTakeAStepEvent(int Action);

    UFUNCTION(BlueprintCallable)
    FGameStateStruct GetState();

    UFUNCTION(BlueprintCallable)
    void SetOldState(FGameStateStruct InOldState);

    UFUNCTION(BlueprintImplementableEvent)
    void SendGameStateToNeuralNetwork();

    UFUNCTION(BlueprintImplementableEvent)
    void StartNeuralNetwork();

    UFUNCTION(BlueprintImplementableEvent)
    void InitMaterialAndColor();

    UFUNCTION(BlueprintImplementableEvent)
    void InitNeuralNetwork();

    UFUNCTION(BlueprintImplementableEvent)
    void SendNetworkData(FNetworkDataStruct NetworkData);


protected:

private:
    float GameLengthInSeconds;
    float EpisodeLengthInSeconds;
    float HighestEpisodeLengthInSeconds;
    TArray<int32> EpisodeLengths;
    TArray<int32> EpisodeLengthsInSeconds;
    int32 HighestEpisodeLength;
    bool bNewInputsReceived;
    AAgent* Enemy;

    FVector2D HittingShot;

    int32 Action;
    int32 BulletsInPlay;
    FGameStateStruct State;
    FGameStateStruct OldState;

    int32 EpisodeLength;
    int32 ResetCounter;

    float ContinueReward;
    float WinReward;
    float LoseReward;

    float MaxLevelHeight;
    float MinLevelHeight;

    int32 Won;
    int32 Lost;

    int32 Minutes;
    int32 Seconds;
    int32 EpisodeMinutes;
    int32 EpisodeSeconds;
    int32 AverageMinutes;
    int32 AverageSeconds;

};
