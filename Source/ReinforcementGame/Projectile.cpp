// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Agents/Agent.h"
#include "Agents/AIAgent.h"
#include "Kismet/GameplayStatics.h"
#include "Agents/NeuralNetworkAgent.h"

AProjectile::AProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> ProjectileMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
     if (ProjectileMeshAsset.Succeeded())
         ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);

    const ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialObject(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));
    if (MaterialObject.Succeeded())
        ProjectileMaterial = MaterialObject.Object;
    DynamicMaterialInstance = UMaterialInstanceDynamic::Create(ProjectileMaterial, GetLevel());

    ProjectileMesh->SetStaticMesh(ProjectileMeshAsset.Object);
    ProjectileMesh->SetupAttachment(RootComponent);
    ProjectileMesh->BodyInstance.SetCollisionProfileName("Projectile");
    ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
    RootComponent = ProjectileMesh;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = ProjectileMesh;
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    InitialLifeSpan = 10.0f;
}

void AProjectile::Init(int InID, FLinearColor ProjectileColor, AAgent* InShooter, AReinforcementGameGameMode* InGameMode)
{
    ID = InID;
    Shooter = InShooter;
    GameMode = InGameMode;
    ProjectileMesh->IgnoreActorWhenMoving(GetOwner(), true);
    UWorld* World = GetWorld();
    TArray<AActor*> FoundAIAgents;
    UGameplayStatics::GetAllActorsOfClass(World, AAIAgent::StaticClass(), FoundAIAgents);
    {
        for (AActor* Actor : FoundAIAgents)
        {
            if (Cast<AAIAgent>(Actor))
            {
                AAIAgent* AIAgent = Cast<AAIAgent>(Actor);
                if (AIAgent->GetID() != ID)
                {
                    ProjectileMesh->IgnoreActorWhenMoving(AIAgent, true);
                    AIAgent->IgnoreActor(this);
                }
            }
        }
    }
    SpawnLocation = FVector2D(GetActorLocation().X, GetActorLocation().Y);
    if (DynamicMaterialInstance != nullptr)
    {
        ProjectileMesh->SetMaterial(0, DynamicMaterialInstance);
        DynamicMaterialInstance->SetVectorParameterValue(TEXT("Color"), ProjectileColor);
    }

    if (GameMode->GetGameMode() == EGameMode::EDestroy)
    {
        InitialSpeed = 2000.0f;
        MaxSpeed = 2000.0f;
    }
    else
    {
        InitialSpeed = 500.0f;
        MaxSpeed = 500.0f;
    }
    ProjectileMovement->InitialSpeed = InitialSpeed;
    ProjectileMovement->MaxSpeed = MaxSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
    if (OtherActor->IsA(AAgent::StaticClass()))
    {
        AAgent* Agent = Cast<AAgent>(OtherActor);
        if (Agent->GetID() != ID)
            Agent->ChangeHealth(-1);
    }
    else
    {
        if(GameMode->GetGameMode() == EGameMode::EDestroy)
            if (Cast<AAgent>(GetOwner()))
            {
                AAgent* Agent = Cast<AAgent>(GetOwner());
                Agent->SetbCanFire(true);
            }
        Destroy();
    }
}

void AProjectile::SetID(int NewID)
{
    ID = NewID;
}
