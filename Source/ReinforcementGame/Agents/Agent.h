// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Agent.generated.h"
class AReinforcementGameGameMode;
class AProjectile;
UENUM(BlueprintType)
enum class EAgentType : uint8
{
    EPlayer,
    EAI,
    ENeuralNetwork
};
UCLASS()
class REINFORCEMENTGAME_API AAgent : public APawn
{
    GENERATED_BODY()

public:
    AAgent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable)
    FORCEINLINE class UStaticMeshComponent* GetMesh() const { return AgentMesh; }
    
    virtual void Init(int ID, FLinearColor AgentColor, EAgentType AgentType, TArray<FVector> WallLocations, int32 InMoveSpeed);
    UFUNCTION(BlueprintCallable)
    int GetID() const { return ID; }
    void SetID(int NewID);
    virtual void Tick(float DeltaTime) override;
    virtual void Fire();
    bool FireLaser();
    void ShotTimerExpired();
    void SetAgentColor(FLinearColor NewColor);
    int GetHealth() const { return Health; }
    void ChangeHealth(int HealthChange);
    virtual void HandleDeath();
    virtual void MoveAgent(float ForwardValue, float RightValue, float DeltaTime);
    void MoveAgentAStep(float ForwardValue, float RightValue, float DeltaTime);
    EAgentType GetAgentType() const { return AgentType; }
    FVector GetSpawnLocation() const { return SpawnLocation; }
    UFUNCTION(BlueprintCallable)
    bool CanAgentFire() const { return bCanFire; }
    void HideActor(bool bShouldHide);
    void ResetActor();
    bool GetLost() const { return bLost; }
    virtual void ResetAgent();
    float GetXVelocity() const { return XVelocity; }
    float GetYVelocity() const { return YVelocity; }
    void SetHitDirections(bool InbHitFront, bool InbHitBehind, bool InbHitLeft, bool InbHitRight);
    void SetbCanFire(bool InbCanFire);
    void IgnoreActor(AActor* Actor);


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Agent)
    int     ID;
    UPROPERTY(EditDefaultsOnly, Category = Agent)
    float   MoveSpeed;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Agent, meta = (AllowPrivateAccess = "true"))
    bool    bCanFire;
    UPROPERTY(EditDefaultsOnly, Category = Agent)
    int     Health;
    int     DefaultHealth;
    UPROPERTY()
    AReinforcementGameGameMode* GameMode;
    UPROPERTY()
    UWorld* World;
    bool bLost;
    TArray<AProjectile*> Projectiles;
    float LaserLength;
    TArray<FVector> WallLocations;
    float XVelocity;
    float YVelocity;
    bool bHitFront;
    bool bHitBehind;
    bool bHitLeft;
    bool bHitRight;
    FVector2D LastShot;

private:
    class UStaticMeshComponent*         AgentMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Agent, meta = (AllowPrivateAccess = "true"))
    class UMaterialInterface*           AgentMaterial;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Agent, meta = (AllowPrivateAccess = "true"))
    class UMaterialInstanceDynamic*     DynamicMaterialInstance;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Agent, meta = (AllowPrivateAccess = "true"))
    FLinearColor    AgentColor;
    FTimerHandle    TimerHandle_ShotTimerExpired;
    FVector         GunOffset;

    float   FireRate;

    EAgentType AgentType;
    FVector SpawnLocation;
    float DefaultWallHeight;
};
