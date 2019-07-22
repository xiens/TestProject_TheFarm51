// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LampSwitch.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ALamp;
class USpotLightComponent;

UCLASS()
class TESTPROJECT_API ALampSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALampSwitch();

	void ToggleLight(); //Call ALamp ToggleLight()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Components")
	ALamp * Lamp = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	FLinearColor ColorOn = FLinearColor(FColor(0, 1, 0));

	UPROPERTY(EditAnywhere, Category = "Components")
	FLinearColor ColorOff = FLinearColor(FColor(1, 0, 0));;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent * SwitchMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent* LightTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpotLightComponent * SpotLight = nullptr;

	bool IsTurnedOn = false;

	void ToggleSwitch(); //Toggle between ColorOn and ColorOff colors
};
