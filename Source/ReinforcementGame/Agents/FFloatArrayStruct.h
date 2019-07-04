// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FFloatArrayStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FFloatArrayStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatArrayStruct")
    TArray<float> FloatArray;

    void Set(TArray<float> InFloatArray)
    {
        FloatArray = InFloatArray;
    }
};
