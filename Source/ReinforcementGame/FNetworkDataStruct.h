// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FGameStateStruct.h"
#include "FNetworkDataStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FNetworkDataStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkDataStruct")
    FGameStateStruct State;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkDataStruct")
    int Action;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkDataStruct")
    FGameStateStruct NewState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkDataStruct")
    float Reward;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkDataStruct")
    bool bDone;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NetworkDataStruct")
    int32 MaxEpisodeLength;

    FNetworkDataStruct()
    {
        State = FGameStateStruct();
        Action = 0;
        Reward = 0.0f;
        NewState = FGameStateStruct();
        bDone = false;
        MaxEpisodeLength = 0;
    }

    void Set(FGameStateStruct InState, int InAction, float InReward, FGameStateStruct InNewState, bool InbDone, int32 InMaxEpisodeLength)
    {
        State = InState;
        Action = InAction;
        NewState = InNewState;
        Reward = InReward;
        bDone = InbDone;
        MaxEpisodeLength = InMaxEpisodeLength;
    }
};
