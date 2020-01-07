// Fill out your copyright notice in the Description page of Project Settings.


#include "GoKart.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "UnrealNetwork.h"

// Sets default values
AGoKart::AGoKart()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
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

	if (HasAuthority()) {
		ReplicatedLocation = GetActorLocation();
		ReplicatedRotation = GetActorRotation();
	}
	else {
		SetActorLocation(ReplicatedLocation);
		SetActorRotation(ReplicatedRotation);
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
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

void AGoKart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AGoKart, ReplicatedLocation);
	DOREPLIFETIME(AGoKart, ReplicatedRotation);
}

void AGoKart::MoveForward(float Value) {
	Throttle = Value;
	Server_MoveForward(Value);
}

void AGoKart::Server_MoveForward_Implementation(float Value) {
	Throttle = Value;
}

bool AGoKart::Server_MoveForward_Validate(float Value) {
	return FMath::Abs(Value) <= 1;
}

void AGoKart::MoveRight(float Value) {
	SteeringThrow = Value;
	Server_MoveRight(Value);
}

void AGoKart::Server_MoveRight_Implementation(float Value) {
	SteeringThrow = Value;
}

bool AGoKart::Server_MoveRight_Validate(float Value) {
	return FMath::Abs(Value) <= 1;
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

FString AGoKart::GetEnumText(ENetRole NetRole) {
	switch (Role) {
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "Simulated proxy";
	case ROLE_AutonomousProxy:
		return "Autonomous Proxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}
