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
	float DmgAmount = 0.1f;
	float DisplaySectionAmount = 0.5f;
	float temp = 0;

	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent * mesh;

	int triangleInd = 0;

	//Mesh properties
	int POINTS = 100;
	float width = 80.0f;
	float height = 150.0f;

	//Mesh components
	TArray<FVector> vertices;
	TArray<int> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	void PostActorCreated();
	void PostLoad();
	void CreateQuad();
	void DestroyQuadSection();

	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices,
		DelaBella_Vertex v,
		std::vector<int>indices, int &oldIndex);

	void CreateTriangle(int i);

};
