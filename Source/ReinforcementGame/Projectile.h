// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReinforcementGameGameMode.h"
#include "Projectile.generated.h"
class AAgent;
UCLASS()
class REINFORCEMENTGAME_API AProjectile : public AActor
{
    GENERATED_BODY()

    UPROPERTY()
    class UStaticMeshComponent* ProjectileMesh;
    UPROPERTY()
    class UProjectileMovementComponent* ProjectileMovement;

public:
    AProjectile();

    void Init(int InID, FLinearColor ProjectileColor, AAgent* InShooter, AReinforcementGameGameMode* InGameMode);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMesh;  }
    FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
    int GetID() const { return ID; }
    void SetID(int NewID);

private:
    int ID;
    AAgent* Shooter;
    FVector2D SpawnLocation;
    AReinforcementGameGameMode* GameMode;
    UPROPERTY(EditDefaultsOnly, Category = Movement)
    float InitialSpeed;
    UPROPERTY(EditDefaultsOnly, Category = Movement)
    float MaxSpeed;

    UPROPERTY()
    class UMaterialInterface* ProjectileMaterial;
    UPROPERTY()
    class UMaterialInstanceDynamic* DynamicMaterialInstance;
};