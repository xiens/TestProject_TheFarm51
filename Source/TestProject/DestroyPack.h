// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "delabella.h"
#include <vector>
#include "ProceduralMeshComponent.h"
#include "DestroyPack.generated.h"

//forward declarations
class UStaticMeshComponent;
class Point2;

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

public:	

	//Called by the engine when actor damage is dealt
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

private:

	float CurrentHealth = 100.0f; //Object's health in %
	float DmgAmount = 0.1f; //Damage caused by every player's hit
	float DisplaySectionAmount = 0.5f; //Amount of damage to hide another mesh section
	float temp = 0; //Temporary variable used in DestroyQuadSections()
	int triangleInd = 0; //Current index of triangle used in CreateTriangle()

	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);

	UPROPERTY(Replicated)
	UProceduralMeshComponent * Mesh;

	//Mesh properties
	int Points = 100;
	float Width = 80.0f;
	float Height = 150.0f;

	//Mesh components
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	//Called after actor is placed in the scene
	void PostActorCreated();

	//Called after loading the game
	void PostLoad();

	//Creates the quad from triangles using Delaunay Triangulation
	void CreateQuad();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestroyQuadSections();
	//Destroys some random sections of quad, when player hits it
	void DestroyQuadSections();

	//Returns the triangle indices needed for mesh creation
	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	//Checks if the vertex v is in the triangleVertices vector, returns also vertex's index
	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices, DelaBella_Vertex v, std::vector<int>indices, int &oldIndex);

	//Adds one triangle section to mesh
	void CreateTriangle(int i);

};
