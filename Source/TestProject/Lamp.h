// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lamp.generated.h"



UENUM(BlueprintType)
enum class ColorEnum : uint8
{
	White =0, 	//UMETA(DisplayName = "White"),
	Yellow = 1, 	//UMETA(DisplayName = "Yellow"),
	Light_Blue = 2,	   // UMETA(DisplayName = "Light_Blue"),
	Red= 3, 	//UMETA(DisplayName = "Red"),
	Green=4 	//UMETA(DisplayName = "Green")
};

//forward declarations
class UStaticMeshComponent;
class USpotLightComponent;

UCLASS()
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

	/** Toggles the light component's visibility*/
	UFUNCTION()
	void ToggleLight();

	/** Changes the light color*/
	UFUNCTION()
	void ChangeColor(FColor color);

private:
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector SwitchLocation = GetActorLocation()+FVector(0,0,50.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent * SwitchMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent * CollisionMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USpotLightComponent * SpotLight = nullptr;


	UPROPERTY(EditAnywhere, Category = "Light properties")
	ColorEnum StartingColor = ColorEnum::White;

	UPROPERTY(EditAnywhere, Category = "Light properties")
	bool IsTurnedOn = false;

	/** light r, g, b components */
	uint8 R, G, B;
	uint8 Max = 250; //max r, g, b value
	uint8 Min = 100; //min r, g, b value
	bool IsAdding = true; //used in ChangeRGB function

	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);

	//** Changes r, g, b values of light color */
	void ChangeRGB(ColorEnum &Colors, uint8 &R, uint8 &G, uint8 &B);

};
