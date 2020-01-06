// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called every frame
void AGoKart::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * DeltaTime;

	Steering(DeltaTime);

	Driving(DeltaTime);
}

// Called to bind functionality to input
void AGoKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGoKart::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGoKart::MoveRight);
}

// Called when the game starts or when spawned
void AGoKart::BeginPlay()
{
	Super::BeginPlay();
}

void AGoKart::MoveForward(float Value) {
	Throttle = Value;
}

void AGoKart::MoveRight(float Value) {
	SteeringThrow = Value;
}

void AGoKart::Driving(float DeltaTime) {
	FVector Translation = Velocity * 100 * DeltaTime; // transform m/s to cm/s
	FHitResult AccHit;
	AddActorWorldOffset(Translation, true, &AccHit);
	if (AccHit.IsValidBlockingHit()) {
		Velocity = FVector(0);
	}
}

void AGoKart::Steering(float DeltaTime) {
	float DeltaLocation = DeltaTime * FVector::DotProduct(Velocity, GetActorForwardVector());
	float RotationAngle = DeltaLocation / TurningCircleRadius * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	AddActorWorldRotation(RotationDelta, true);
	Velocity = RotationDelta.RotateVector(Velocity);
}

FVector AGoKart::GetAirResistance() {
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector AGoKart::GetRollingResistance() {
	float NormalForce = -(Mass * GetWorld()->GetGravityZ() / 100);
	return  -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}
