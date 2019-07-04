// Fill out your copyright notice in the Description page of Project Settings.

#include "Agent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Projectile.h"
#include "TimerManager.h"
#include "ReinforcementGameGameMode.h"
#include "Kismet/GameplayStatics.h"

AAgent::AAgent()
{
    PrimaryActorTick.bCanEverTick = true;

    AgentColor = FLinearColor::Blue;

    AgentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AgentMesh"));

    const ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("/Engine/BasicShapes/Cube"));
    if (MeshObject.Succeeded())
        AgentMesh->SetStaticMesh(MeshObject.Object);

    const ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObject(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
    if (MaterialObject.Succeeded())
        AgentMaterial = MaterialObject.Object;
    DynamicMaterialInstance = UMaterialInstanceDynamic::Create(AgentMaterial, GetLevel());

    MoveSpeed = 1000.0f;
    GunOffset = FVector(90.0f, 0.f, 0.f);

    bCanFire = true;
    Health = 1;
    DefaultHealth = Health;
    bLost = false;
    LaserLength = 900.0f;
    XVelocity = 0.0f;
    YVelocity = 0.0f;
    LastShot = FVector2D(GetActorLocation().X, GetActorLocation().Y);

    RootComponent = AgentMesh;

}

void AAgent::BeginPlay()
{
    Super::BeginPlay();
    SpawnLocation = GetActorLocation();

}

void AAgent::Init(int ID, FLinearColor AgentColor, EAgentType AgentType, TArray<FVector> WallLocations, int32 InMoveSpeed)
{
    this->ID = ID;
    this->AgentColor = AgentColor;
    this->AgentType = AgentType;
    this->WallLocations = WallLocations;
    MoveSpeed = InMoveSpeed;
    World = GetWorld();
    if (World)
        GameMode = (AReinforcementGameGameMode*)World->GetAuthGameMode();
    if (DynamicMaterialInstance != nullptr)
    {
        AgentMesh->SetMaterial(0, DynamicMaterialInstance);
        DynamicMaterialInstance->SetVectorParameterValue(TEXT("Color"), AgentColor);
    }
    if (GameMode->GetGameMode() == EGameMode::ESurviveHard)
        FireRate = 0.25f;
    else
        FireRate = 0.14f;
}

void AAgent::SetID(int NewID)
{
    ID = NewID;
}

void AAgent::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AAgent::Fire()
{
    if (bCanFire && GameMode->GetPlayState() == EPlayState::EPlay)
    {
        int FireDirection;
        if(GetActorLocation().X > WallLocations[2].X)
            FireDirection = -1;
        else
            FireDirection = 1;

        FVector ForwardVector = FVector(FireDirection * 500.0f, 0.0f, 0.0f);
        FRotator Rotation = FireDirection*ForwardVector.Rotation();
        const FVector SpawnLocation = GetActorLocation() + Rotation.RotateVector(GunOffset);
        UWorld* const World = GetWorld();
        if (World)
        {
            FTransform SpawnTransform(Rotation, SpawnLocation);
            AProjectile* Projectile = Cast<AProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, AProjectile::StaticClass(), SpawnTransform
                                                        , ESpawnActorCollisionHandlingMethod::Undefined, this));
            if (Projectile != nullptr)
            {
                Projectile->Init(ID, AgentColor, this, GameMode);
                Cast<AProjectile>(UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform));
            }
            LastShot = FVector2D(SpawnLocation.X, SpawnLocation.Y);
            World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AAgent::ShotTimerExpired, FireRate);
            bCanFire = false;
        }
    }
}

bool AAgent::FireLaser()
{
    bool bHit = false;
    if (GameMode->GetPlayState() == EPlayState::EPlay)
    {
        for (AAgent* Agent : GameMode->GetAgents())
        {
            if (Agent->GetID() != ID)
            {
                float YDistanceToEnemy = FMath::Pow((GetActorLocation().Y - Agent->GetActorLocation().Y), 2.0f);
                float XDistanceToEnemy = FMath::Pow((GetActorLocation().X - Agent->GetActorLocation().X), 2.0f);
                if(YDistanceToEnemy <= FMath::Pow(50.0f, 2.0f) && XDistanceToEnemy <= FMath::Pow(LaserLength, 2.0f))
                    bHit = true;
            }
        }
    }
    return bHit;
}

void AAgent::ShotTimerExpired()
{
    bCanFire = true;
}

void AAgent::SetAgentColor(FLinearColor NewColor)
{
    AgentColor = NewColor;
}

void AAgent::ChangeHealth(int HealthChange)
{
    Health += HealthChange;
    if (Health <= 0)
        HandleDeath();
}

void AAgent::HandleDeath()
{
    HideActor(true);
    bLost = true;
}

void AAgent::MoveAgent(float ForwardValue, float RightValue, float DeltaTime)
{
    const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.0f).GetClampedToMaxSize(1.0f);
    const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;
    XVelocity = Movement.X;
    YVelocity = Movement.Y;

    if (Movement.SizeSquared() > 0.0f)
    {
        const FRotator NewRotation = Movement.Rotation();
        RootComponent->MoveComponent(Movement, GetActorRotation(), true);
    }
}

void AAgent::MoveAgentAStep(float ForwardValue, float RightValue, float DeltaTime)
{
    const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.0f).GetClampedToMaxSize(1.0f);
    const FVector Movement = MoveDirection * MoveSpeed * DeltaTime;

    FVector Location = GetActorLocation();
    if (RightValue < 0 && Location.Y > 150.0)
    {
        FVector NewLocation(Location.X, Location.Y + (RightValue * 25.0), Location.Z);
        SetActorLocation(NewLocation);
    }
    if (RightValue > 0 && Location.Y < 650.0)
    {
        FVector NewLocation(Location.X, Location.Y + (RightValue * 25.0), Location.Z);
        SetActorLocation(NewLocation);
    }
}

void AAgent::HideActor(bool bShouldHide)
{
    SetActorHiddenInGame(bShouldHide);
    SetActorEnableCollision(!bShouldHide);
}

void AAgent::ResetActor()
{
    HideActor(true);
    float X = 0.0f;
    float Y = 0.0f;
    float MiddleWallX = WallLocations[2].X;
    float RightWallY = WallLocations[3].Y;
    float LeftWallY = WallLocations[4].Y;
    if (AgentType == EAgentType::ENeuralNetwork)
    {
        float TopWallX = WallLocations[0].X;
        X = FMath::RandRange(MiddleWallX + ((TopWallX - MiddleWallX) / 2), TopWallX - 175.0f);
    }
    else
    {
        float BottomWallX = WallLocations[1].X;
        if (GameMode->GetGameMode() == EGameMode::ESurviveHard)
        {
            X = SpawnLocation.X;
        }
        else
        {
            X = FMath::RandRange(BottomWallX + 175.0f, MiddleWallX - ((MiddleWallX - BottomWallX) / 2));
        }
    }
    Y = FMath::RandRange(LeftWallY + 175.0f, RightWallY - 175.0f);
    SetActorLocation(FVector(X, Y, SpawnLocation.Z));
    LastShot = FVector2D(GetActorLocation().X, GetActorLocation().Y);
    HideActor(false);
}

void AAgent::ResetAgent()
{
    bLost = false;
    bCanFire = true;
    Health = DefaultHealth;
    SetHitDirections(false, false, false, false);
}

void AAgent::SetHitDirections(bool InbHitFront, bool InbHitBehind, bool InbHitLeft, bool InbHitRight)
{
    bHitFront = InbHitFront;
    bHitBehind = InbHitBehind;
    bHitLeft = InbHitLeft;
    bHitRight = InbHitRight;
}

void AAgent::SetbCanFire(bool InbCanFire)
{
    bCanFire = InbCanFire;
}

void AAgent::IgnoreActor(AActor * Actor)
{
    AgentMesh->IgnoreActorWhenMoving(Actor, true);
}
