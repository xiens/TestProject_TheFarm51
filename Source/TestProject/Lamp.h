// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lamp.generated.h"

UENUM(BlueprintType)
enum class ColorEnum : uint8
{
	White = 0 		UMETA(DisplayName = "White"),
	Yellow = 1	 	UMETA(DisplayName = "Yellow"),
	Light_Blue = 2	UMETA(DisplayName = "Light_Blue"),
	Red= 3 			UMETA(DisplayName = "Red"),
	Green= 4 		UMETA(DisplayName = "Green")
};

//forward declarations
class UStaticMeshComponent;
class USpotLightComponent;
class USphereComponent;

UCLASS(ShowFunctions=ToggleLight, meta = (ToolTip = "Press F key to toggle the light on/off"))
class TESTPROJECT_API ALamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Toggles the light component's visibility
	//Press F key to toggle the light on/off
	UFUNCTION()
	void ToggleLight();

	//Changes the light color
	UFUNCTION()
	void ChangeColor(FColor color);

private:
	//Location of SwitchMesh and LightTrigger
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector SwitchLocation = GetActorLocation()+FVector(20.0f,0,0);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent * SwitchMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USphereComponent* LightTrigger;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent * CollisionMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpotLightComponent * SpotLight = nullptr;

	UPROPERTY(EditAnywhere, Category = "Light properties")
	ColorEnum StartingColor = ColorEnum::White;

	UPROPERTY(EditAnywhere, Category = "Light properties")
	float LightChangeRate = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Light properties")
	float LightIntensity = 50000.0f;

	ColorEnum FindNextColor(ColorEnum &NextColor, FVector &NextColorVal);
	FVector FindColorVal(ColorEnum &Color);
	void ChangeColorOverTime(float DeltaTime);

	//some predefined values for colors
	FVector White = FVector(1, 1, 1);
	FVector Yellow = FVector(1, 1, 0);
	FVector Light_Blue = FVector(0, 0.8f, 0.85f);
	FVector Red = FVector(1, 0, 0);
	FVector Green = FVector(0, 1, 0);

	//time used in lerp, changes depending on LightChangeRate and DeltaTime
	float time = 1.1f;

	ColorEnum CurrentColor = StartingColor;
	ColorEnum NextColor = StartingColor;

	FVector CurrentColorVal = FindColorVal(StartingColor);
	FVector NextColorVal = CurrentColorVal;
};
