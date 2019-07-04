// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelCube.h"
#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

ALevelCube::ALevelCube()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObject.Succeeded())
        Mesh->SetStaticMesh(MeshObject.Object);

    const ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObject(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
    if (MaterialObject.Succeeded())
        Material = MaterialObject.Object;
    DynamicMaterialInstance = UMaterialInstanceDynamic::Create(Material, GetLevel());

    if (DynamicMaterialInstance != nullptr)
    {
        Mesh->SetMaterial(0, DynamicMaterialInstance);
        DynamicMaterialInstance->SetVectorParameterValue(TEXT("Color"), FLinearColor::White);
    }

    RootComponent = Mesh;
}