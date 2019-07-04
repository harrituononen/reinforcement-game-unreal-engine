// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class REINFORCEMENTGAME_API ACameraPawn : public APawn
{
    GENERATED_BODY()

public:
    ACameraPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    void SetLocationAndRotation(FVector NewLocation, FRotator NewRotation);
    FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

private:
    UPROPERTY()
    class UCameraComponent* CameraComponent;

};
