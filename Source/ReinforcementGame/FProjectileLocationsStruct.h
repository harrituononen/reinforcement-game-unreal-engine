// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "FProjectileLocationsStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FProjectileLocationsStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileLocationsStruct")
    float X;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileLocationsStruct")
    float Y;

    FProjectileLocationsStruct()
    {
        X = 0.0f;
        Y = 0.0f;
    }

    void Set(float InX, float InY)
    {
        X = InX;
        Y = InY;
    }
};
