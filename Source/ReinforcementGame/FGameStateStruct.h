// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Agents/FFloatArrayStruct.h"
#include "FProjectileLocationsStruct.h"
#include "FGameStateStruct.generated.h"


USTRUCT(BlueprintType)
struct FGameStateStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float MyX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float MyY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float EnemyX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float EnemyY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D MyLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D EnemyLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D ClosestProjectileLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D SecondClosestProjectileLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D ThirdClosestProjectileLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D ClosestProjectileLocationToEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D LastShot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    FVector2D HittingShot;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float EnemyXVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float EnemyYVelocity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    float DistanceToEnemyX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    int32 ShotBulletsInPlay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletHeadingTowardsEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletInFront;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletBehind;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletInFrontOfEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletBehindEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletLeftOfEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bBulletRightOfEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bMovingUp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bMovingDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bMovingLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bMovingRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyMovingUp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyMovingDown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyMovingLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyMovingRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bTooFarAway;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bEnemyFired;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bHitFront;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bHitBehind;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bHitLeft;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bHitRight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bCloseToTopWall;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bCloseToBottomWall;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bCloseToMiddleWall;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bCloseToRightWall;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bCloseToLeftWall;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameStateStruct")
    bool bCanFire;

    FGameStateStruct()
    {
        MyX = 0.0f;
        MyY = 0.0f;
        EnemyX = 0.0f;
        EnemyY = 0.0f;
        MyLocation = FVector2D(0.0f,0.0f);
        EnemyLocation = FVector2D(0.0f,0.0f);
        ClosestProjectileLocation = FVector2D(0.0f, 0.0f);
        SecondClosestProjectileLocation = FVector2D(0.0f, 0.0f);
        ThirdClosestProjectileLocation = FVector2D(0.0f, 0.0f);
        ClosestProjectileLocationToEnemy = FVector2D(0.0f, 0.0f);
        LastShot = FVector2D(0.0f, 0.0f);
        HittingShot = FVector2D(0.0f, 0.0f);
        EnemyXVelocity = 0.0f;
        EnemyYVelocity = 0.0f;
        DistanceToEnemyX = 0.0f;
        ShotBulletsInPlay = 0;
        bBulletHeadingTowardsEnemy = false;
        bBulletInFront = false;
        bBulletBehind = false;
        bBulletLeft = false;
        bBulletRight = false;
        bBulletInFrontOfEnemy = false;
        bBulletBehindEnemy = false;
        bBulletLeftOfEnemy = false;
        bBulletRightOfEnemy = false;
        bMovingUp = false;
        bMovingDown = false;
        bMovingLeft = false;
        bMovingRight = false;
        bEnemyMovingUp = false;
        bEnemyMovingDown = false;
        bEnemyMovingLeft = false;
        bEnemyMovingRight = false;
        bTooFarAway = false;
        bEnemyLeft = false;
        bEnemyRight = false;
        bEnemyFired = false;
        bHitFront = false;
        bHitBehind = false;
        bHitLeft = false;
        bHitRight = false;
        bCloseToTopWall = false;
        bCloseToBottomWall = false;
        bCloseToMiddleWall = false;
        bCloseToRightWall = false;
        bCloseToLeftWall = false;
        bCanFire = false;
    }

    void Set(float InMyX, float InMyY, float InEnemyX, float InEnemyY, FVector2D InMyLocation, FVector2D InEnemyLocation
        , FVector2D InClosestProjectileLocation, FVector2D InSecondClosestProjectileLocation, FVector2D InThirdClosestProjectileLocation
        , FVector2D InClosestProjectileLocationToEnemy, FVector2D InLastShot, FVector2D InHittingShot, float InEnemyXVelocity, float InEnemyYVelocity
        , float InDistanceToEnemyX, int32 InShotBulletsInPlay, bool InbBulletHeadingTowardsEnemy
        , bool InbBulletInFront, bool InbBulletBehind, bool InbBulletLeft, bool InbBulletRight, bool InbBulletInFrontOfEnemy, bool InbBulletBehindEnemy
        , bool InbBulletLeftOfEnemy, bool InbBulletRightOfEnemy
        , bool InbMovingUp, bool InbMovingDown, bool InbMovingLeft, bool InbMovingRight
        , bool InbEnemyMovingUp, bool InbEnemyMovingDown, bool InbEnemyMovingLeft, bool InbEnemyMovingRight, bool InbTooFarAway, bool InbEnemyLeft
        , bool InbEnemyRight, bool InbEnemyFired
        , bool InbHitFront, bool InbHitBehind, bool InbHitLeft, bool InbHitRight
        , bool InbCloseToTopWall, bool InbCloseToBottomWall, bool InbCloseToMiddleWall, bool InbCloseToRightWall, bool InbCloseToLeftWall
        , bool InbCanFire)
    {
        MyX = InMyX;
        MyY = InMyY;
        EnemyX = InEnemyX;
        EnemyY = InEnemyY;
        MyLocation = InMyLocation;
        EnemyLocation = InEnemyLocation;
        ClosestProjectileLocation = InClosestProjectileLocation;
        SecondClosestProjectileLocation = InSecondClosestProjectileLocation;
        ThirdClosestProjectileLocation = InThirdClosestProjectileLocation;
        ClosestProjectileLocationToEnemy = InClosestProjectileLocationToEnemy;
        LastShot = InLastShot;
        HittingShot = InHittingShot;
        EnemyXVelocity = InEnemyXVelocity;
        EnemyYVelocity = InEnemyYVelocity;
        DistanceToEnemyX = InDistanceToEnemyX;
        ShotBulletsInPlay = InShotBulletsInPlay;
        bBulletHeadingTowardsEnemy = InbBulletHeadingTowardsEnemy;
        bBulletInFront = InbBulletInFront;
        bBulletBehind = InbBulletBehind;
        bBulletLeft = InbBulletLeft;
        bBulletRight = InbBulletRight;
        bBulletInFrontOfEnemy = InbBulletInFrontOfEnemy;
        bBulletBehindEnemy = InbBulletBehindEnemy;
        bBulletLeftOfEnemy = InbBulletLeftOfEnemy;
        bBulletRightOfEnemy = InbBulletRightOfEnemy;
        bMovingUp = InbMovingUp;
        bMovingDown = InbMovingDown;
        bMovingLeft = InbMovingLeft;
        bMovingRight = InbMovingRight;
        bEnemyMovingUp = InbEnemyMovingUp;
        bEnemyMovingDown = InbEnemyMovingDown;
        bEnemyMovingLeft = InbEnemyMovingLeft;
        bEnemyMovingRight = InbEnemyMovingRight;
        bTooFarAway = InbTooFarAway;
        bEnemyLeft = InbEnemyLeft;
        bEnemyRight = InbEnemyRight;
        bEnemyFired = InbEnemyFired;
        bHitFront = InbHitFront;
        bHitBehind = InbHitBehind;
        bHitLeft = InbHitLeft;
        bHitRight = InbHitRight;
        bCloseToTopWall = InbCloseToTopWall;
        bCloseToBottomWall = InbCloseToBottomWall;
        bCloseToMiddleWall = InbCloseToMiddleWall;
        bCloseToRightWall = InbCloseToRightWall;
        bCloseToLeftWall = InbCloseToLeftWall;
        bCanFire = InbCanFire;
    }
};
