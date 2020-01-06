// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GoKart.generated.h"

UCLASS()
class S04_KRAZYKARTS_API AGoKart : public APawn
{
	GENERATED_BODY()

/** Properties */
private:
	FVector Velocity; // in m/s

	float Throttle;
	float SteeringThrow;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;

	// In m
	UPROPERTY(EditAnywhere)
	float TurningCircleRadius = 10;

	// Mass in kg
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.03;

/** Methods */
public:
	// Sets default values for this pawn's properties
	AGoKart();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float Value);

	void Driving(float DeltaTime);

	void Steering(float DeltaTime);

	FVector GetAirResistance();

	FVector GetRollingResistance();

};
