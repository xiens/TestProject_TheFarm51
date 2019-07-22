// Fill out your copyright notice in the Description page of Project Settings.


#include "LampSwitch.h"
#include "Lamp.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values
ALampSwitch::ALampSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Switch Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwitchAsset(TEXT("/Game/StarterContent/Architecture/SM_AssetPlatform.SM_AssetPlatform"));
	if (SwitchAsset.Succeeded()) {

		SwitchMesh->SetStaticMesh(SwitchAsset.Object);
		SwitchMesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	RootComponent = SwitchMesh;

	LightTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Light Sphere Component"));
	LightTrigger->InitSphereRadius(1000.0f);
	LightTrigger->SetCollisionProfileName(TEXT("Trigger"));
	LightTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	LightTrigger->SetupAttachment(RootComponent);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(FName("SpotLight"));
	SpotLight->bVisible = true;
	SpotLight->SetupAttachment(RootComponent);
	SpotLight->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SpotLight->SetRelativeLocation(FVector(0.0f, 0.0f, 105.0f));
}

// Called when the game starts or when spawned
void ALampSwitch::BeginPlay()
{
	Super::BeginPlay();
	SpotLight->Activate(true);
	SpotLight->SetLightColor(ColorOff);
}

void ALampSwitch::ToggleSwitch() {
	IsTurnedOn = !IsTurnedOn;

	if (IsTurnedOn) {
		SpotLight->SetLightColor(ColorOn);
	}
	else{ 
		SpotLight->SetLightColor(ColorOff); 
	}
}


void ALampSwitch::ToggleLight() {
	if (Lamp) {
		Lamp->ToggleLight();
		ToggleSwitch();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Lamp selected for object: %s"), *this->GetName())
	}
}

