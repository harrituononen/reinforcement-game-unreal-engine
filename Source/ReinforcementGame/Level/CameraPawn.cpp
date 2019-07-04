// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraPawn.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"

ACameraPawn::ACameraPawn()
{
    PrimaryActorTick.bCanEverTick = false;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    RootComponent = CameraComponent;
    CameraComponent->Activate();
}

void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ACameraPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACameraPawn::SetLocationAndRotation(FVector NewLocation, FRotator NewRotation)
{
    CameraComponent->SetWorldLocationAndRotation(NewLocation, NewRotation);
}

