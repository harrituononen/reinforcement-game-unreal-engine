// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "LevelCube.generated.h"

/**
 * 
 */
UCLASS()
class REINFORCEMENTGAME_API ALevelCube : public AStaticMeshActor
{
    GENERATED_BODY()
    
public:
    ALevelCube();

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelCube, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent*         Mesh;
    UPROPERTY(EditAnywhere, Category = LevelCube)
    class UMaterialInterface*           Material;
    UPROPERTY(EditAnywhere, Category = LevelCube)
    class UMaterialInstanceDynamic*     DynamicMaterialInstance;
};
