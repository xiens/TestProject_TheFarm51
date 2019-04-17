// Fill out your copyright notice in the Description page of Project Settings.

#include "Lamp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
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
		//CollisionMesh->SetRelativeRotation(FRotator(180.0f, 0.0f, 0.0f));
	}
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwitchAsset(TEXT("/Game/StarterContent/Architecture/SM_AssetPlatform.SM_AssetPlatform"));
	if (SwitchAsset.Succeeded()) {
		
		SwitchMesh->SetStaticMesh(SwitchAsset.Object);
		SwitchMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	}


	RootComponent = CollisionMesh;
	CollisionMesh->SetNotifyRigidBodyCollision(true);

	

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(FName("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, -125.0f));

}

// Called when the game starts or when spawned
void ALamp::BeginPlay()
{
	Super::BeginPlay();
	R = Min + 1; //FMath::FRandRange(0, 255);
	G = Min + 1; //FMath::FRandRange(0, 255);
	B = Min + 1; //FMath::FRandRange(0, 255);

	SwitchMesh->SetupAttachment(RootComponent);
	SwitchMesh->SetRelativeLocation(SwitchLocation);
	SwitchMesh->SetNotifyRigidBodyCollision(true);
	SwitchMesh->OnComponentHit.AddDynamic(this, &ALamp::OnHit);
}

// Called every frame
void ALamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
		
	ChangeRGB(StartingColor, R, G, B);
	FColor Color = FColor(R, G, B, 255);
	ChangeColor(Color);
}

void ALamp::ToggleLight()
{
	SpotLight->ToggleActive();
}

void ALamp::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("light toggle"))
	ToggleLight();
}

void ALamp::ChangeRGB(ColorEnum &Colors, uint8 &R, uint8 &G, uint8 &B)
{
	switch (Colors) {
	case ColorEnum::White: {

		if (R >= Max) IsAdding = false;
		if (R <= Min) { IsAdding = true; Colors = ColorEnum::Green; }

		if (R < Max &&  IsAdding) {
			R++;
			G++;
			B++;
		}
		else if (R > Min && !IsAdding) {
			R--;
			G--;
			B--;

		}

		break;
		}
		case ColorEnum::Green:{

			if (G >= Max) IsAdding = false;
			if (G <= Min) { IsAdding = true; Colors = ColorEnum::Light_Blue;}

			if (G < Max && IsAdding) {
				G++;

			}
			else if (G > Min && !IsAdding) {
				G--;
			}

			break;
		}
		case ColorEnum::Light_Blue: {

			if (B >= Max) IsAdding = false;
			if (B <= Min) { IsAdding = true; Colors = ColorEnum::Yellow;}

			if (B < Max && IsAdding) {
				B += 2;
				G++;

			}
			else if (B > Min && !IsAdding) {
				B -= 2;
				G--;
			}

			break;
		}
		case ColorEnum::Yellow: {

			if (R >= Max) IsAdding = false;
			if (R <= Min) { IsAdding = true; Colors = ColorEnum::Red; }

			if (B < Max && IsAdding) {
				R++;
				G++;

			}
			else if (B > Min && !IsAdding) {
				R--;
				G--;
			}

			break;
		}
		case ColorEnum::Red: {

			if (R >= Max) IsAdding = false;
			if (R <= Min) { IsAdding = true; Colors = ColorEnum::White; }

			if (R < Max &&  IsAdding) {
				R++;
			}
			else if (R > Min && !IsAdding) {
				R--;
			}

			break;
		}

	}
}

void ALamp::ChangeColor(FColor Color)
{
	FLinearColor ColorToChange = FLinearColor(Color);
	SpotLight->SetLightColor(ColorToChange);
}
