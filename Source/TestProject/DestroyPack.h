// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "delabella.h"
#include <vector>
#include "ProceduralMeshComponent.h"
#include "DestroyPack.generated.h"

class UStaticMeshComponent;
class Point2;
class MeshGenerator;
class UBoxComponent;

/**
* Destroyable object with health
* Uses mesh created in the MeshGenerator class
* Each TestProjectCharacter's hit hides some triangles of this object's mesh
*/
UCLASS()
class TESTPROJECT_API ADestroyPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestroyPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	UPROPERTY(EditAnywhere, Category = "Components")
	UProceduralMeshComponent * Mesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Components")
	UBoxComponent * BoxComponent = nullptr;

	MeshGenerator * MeshGen = nullptr;

	const int Points = 200; //Number of points, used in triangulation
	const float Width = 100.0f; //Width of generated quad
	const float Height = 450.0f; //Height of generated quad

	const float DmgAmount = 0.1f; //Damage caused by every player's hit
	const float HideSectionAmount = 0.5f; //Amount of damage to hide another mesh section

	float CurrentHealth = 100.0f; //Object's health in %
	float temp = 0; //Temporary variable used in DestroyQuadSections()

	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);

	//Called after actor is placed in the scene
	void PostActorCreated();

	//Called after loading the game
	void PostLoad();

	//Destroys some random sections of quad, called when player hits this object
	void DestroyQuadSections();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestroyQuadSections();



};
