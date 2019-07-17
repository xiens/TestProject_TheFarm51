// Fill out your copyright notice in the Description page of Project Settings.

#include "Lamp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/SphereComponent.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "Engine/Public/DrawDebugHelpers.h "
#include "UnrealNetwork.h"

// Sets default values
ALamp::ALamp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Switch Mesh"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CeilingLampAsset(TEXT("/Game/StarterContent/Props/SM_Lamp_Ceiling.SM_Lamp_Ceiling")); 
	if (CeilingLampAsset.Succeeded())
	{
		CollisionMesh->SetStaticMesh(CeilingLampAsset.Object);
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwitchAsset(TEXT("/Game/StarterContent/Architecture/SM_AssetPlatform.SM_AssetPlatform"));
	if (SwitchAsset.Succeeded()) {
		
		SwitchMesh->SetStaticMesh(SwitchAsset.Object);
		SwitchMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		SwitchMesh->SetupAttachment(RootComponent);
	}
	RootComponent = CollisionMesh;
	CollisionMesh->SetNotifyRigidBodyCollision(true);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(FName("SpotLight"));
	SpotLight->bVisible = true;
	SpotLight->Intensity = LightIntensity;
	SpotLight->SetupAttachment(RootComponent);
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, -125.0f));

	LightTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Light Sphere Component"));
	LightTrigger->InitSphereRadius(100.0f);
	LightTrigger->SetCollisionProfileName(TEXT("Trigger"));
	LightTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	LightTrigger->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALamp::BeginPlay()
{
	Super::BeginPlay();

	SwitchMesh->SetRelativeLocation(SwitchLocation);
	LightTrigger->SetRelativeLocation(SwitchLocation);
}

// Called every frame
void ALamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeColorOverTime(DeltaTime);
}

void ALamp::ChangeColorOverTime(float DeltaTime)
{
	if (time > 1.0f) {
		time = 0.0f;
		CurrentColor = NextColor;
		CurrentColorVal = NextColorVal;
		NextColor = FindNextColor(CurrentColor, NextColorVal);
	}

	time += DeltaTime * LightChangeRate;
	FVector Col = FMath::Lerp(CurrentColorVal, NextColorVal, time);


	FColor Color = FColor(Col.X * 255, Col.Y * 255, Col.Z * 255, 255);
	ChangeColor(Color);
}

void ALamp::ChangeColor(FColor Color)
{
	FLinearColor ColorToChange = FLinearColor(Color);
	SpotLight->SetLightColor(ColorToChange);
}

ColorEnum ALamp::FindNextColor(ColorEnum &Color, FVector &ColorVal)
{
	switch (Color) {
		case ColorEnum::White: {
			ColorVal = Green;
			return ColorEnum::Green;
			break;
		}
		case ColorEnum::Green: {
			ColorVal = Light_Blue;
			return ColorEnum::Light_Blue;
			break;
		}
		case ColorEnum::Light_Blue: {
			ColorVal = Yellow;
			return ColorEnum::Yellow;
			break;
		}
		case ColorEnum::Yellow: {
			ColorVal = Red;
			return ColorEnum::Red;
			break;
		}
		case ColorEnum::Red: {
			ColorVal = White;
			return ColorEnum::White;
			break;
		}
	}
	return ColorEnum::White;
}

FVector ALamp::FindColorVal(ColorEnum & Color)
{
	switch (Color) {
		case ColorEnum::White: {
			return White;
			break;
		}
		case ColorEnum::Green: {
			return Green;
			break;
		}
		case ColorEnum::Light_Blue: {
			return Light_Blue;
			break;
		}
		case ColorEnum::Yellow: {
			return Yellow;
			break;
		}
		case ColorEnum::Red: {
			return Red;
			break;
		}
	}

	return FVector(1,1,1); //white value as default
}

void ALamp::ToggleLight()
{
	SpotLight->ToggleActive();
	SpotLight->ToggleVisibility();
}