// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyPack.h"
#include "Components/StaticMeshComponent.h"
#include "Public\Point2.h"
#include <vector>
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "UnrealNetwork.h"

// Sets default values
ADestroyPack::ADestroyPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	Mesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
	Mesh->bUseAsyncCooking = true;
	Mesh->SetNotifyRigidBodyCollision(true);

	RootComponent = Mesh;
}

// Called when the game starts or when spawned
void ADestroyPack::BeginPlay()
{
	Super::BeginPlay();
	Mesh->OnComponentHit.AddDynamic(this, &ADestroyPack::OnHit);
}

void ADestroyPack::PostActorCreated()
{
	Super::PostActorConstruction();
	UE_LOG(LogTemp, Warning, TEXT("PostActorConstruction"))
	CreateQuad();
}

void ADestroyPack::PostLoad()
{
	Super::PostLoad();
}

void ADestroyPack::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADestroyPack, Mesh);
}

float ADestroyPack::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Table hit"))
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp<float>(DamageAmount, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("Current health is: %f"), CurrentHealth)

		if (CurrentHealth <= 0) {
			UE_LOG(LogTemp, Warning, TEXT("Table destroyed"))
				Destroy();
		}
	UE_LOG(LogTemp, Warning, TEXT("Damage: %f, DamageToApply %i"), DamageAmount, DamageToApply)
		
	return DamageToApply;
}

void ADestroyPack::DestroyQuadSections()
{
	CurrentHealth -= DmgAmount;

	temp += DmgAmount;
	int32 i = FMath::RandRange(1, Points - 1);

	if (temp >= DisplaySectionAmount) {
		temp = 0;
		Mesh->SetMeshSectionVisible(i, false);
	}

	if (CurrentHealth < 0) {
		Mesh->DestroyComponent();
		CurrentHealth = 100.0f;
	}
	Server_DestroyQuadSections();
}

void ADestroyPack::Server_DestroyQuadSections_Implementation()
{
	CurrentHealth -= DmgAmount;
	temp += DmgAmount;
	int32 i = FMath::RandRange(1, Points - 1);

	if (temp >= DisplaySectionAmount) {
		temp = 0;
		Mesh->SetMeshSectionVisible(i, false);
	}

	if (CurrentHealth < 0) {
			Mesh->DestroyComponent();
		CurrentHealth = 100.0f;
	}

}

bool ADestroyPack::Server_DestroyQuadSections_Validate()
{
	return true;
}

void ADestroyPack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	DestroyQuadSections();
}



void ADestroyPack::CreateQuad()
{
	Point2* cloud = new Point2[Points];

	if (Points < 5) return;

	cloud[0] = Point2(-0.5f*Width, -0.5f*Height);
	cloud[1] = Point2(0.5f*Width, -0.5f*Height);
	cloud[3] = Point2(-0.5f*Width, 0.5f*Height);
	cloud[2] = Point2(0.5f*Width, 0.5f*Height);
	cloud[4] = Point2(-0.5f*Width, -0.501f*Height);

	// gen some random input
	for (int i = 5; i < Points; i++)
	{
		cloud[i].x = FMath::FRandRange(-0.5f * Width, 0.5f * Width);
		cloud[i].y = FMath::FRandRange(-0.5f * Height, 0.5f * Height);
	}

	IDelaBella* idb = IDelaBella::Create();

	int verts = idb->Triangulate(Points, &cloud->x, &cloud->y, sizeof(Point2));

	std::vector<DelaBella_Triangle> triangles;
	std::vector<DelaBella_Vertex> triangleVertices;  //All unique vertices of triangles

		//if positive, all ok 
		if (verts > 0)
		{
			int tris = verts / 3;
			const DelaBella_Triangle* dela = idb->GetFirstDelaunayTriangle();

			for (int i = 0; i < tris; i++)
			{
				triangles.push_back(*dela);
				dela = dela->next;
			}
		}
		else
		{
			// no points given or all points are colinear
			UE_LOG(LogTemp, Warning, TEXT("no points given or all points are colinear"))
		}

	std::vector<int> triangleIndices;

	triangleIndices = CalculateTriangleIndices(triangles, triangleVertices);

	for (int i = 0; i < triangleIndices.size(); i++)
	{
		Triangles.Add(triangleIndices[i]);
	}

	for (int i = 0; i < triangleVertices.size(); i++)
	{
		Normals.Add(FVector(1, 0, 0));
		UV0.Add(FVector2D(triangleVertices[i].x, triangleVertices[i].y));
		Vertices.Add(FVector(triangleVertices[i].x, triangleVertices[i].y, 0));
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(1, 0, 0, 1.0));
	}

	//Create section for every triangle in our mesh
	int cnt = 0;
	triangleInd = 1;

	for (int i = 0; i < triangleIndices.size()/3; i++) {
		CreateTriangle(cnt);
		cnt += 3;
	}

	// Enable collision data
	Mesh->ContainsPhysicsTriMeshData(true);
}

std::vector<int> ADestroyPack::CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex>& triangleVertices)
{
	std::vector<int> allIndices;; //All unique indices of vertices of triangles
	std::vector<int> indices; //vertices of triangles in proper order for creating mesh

	DelaBella_Vertex currentVertex = *triangles[0].v[0];

	int triangleCnt = 0;
	int vertexInd = 0;
	int i = 0;
	int oldIndex = 0;

	while (triangleCnt != triangles.size())
	{

		if (vertexInd == 0) currentVertex = *triangles[triangleCnt].v[0];
		else if (vertexInd == 1) currentVertex = *triangles[triangleCnt].v[1];
		else if (vertexInd == 2) currentVertex = *triangles[triangleCnt].v[2];


		if (IsVertexDefined(triangleVertices, currentVertex, allIndices, oldIndex))
		{
			indices.push_back(oldIndex);
		}
		else
		{
			indices.push_back(i);
			allIndices.push_back(i);
			triangleVertices.push_back(currentVertex);
			i++;
		}
		vertexInd++; //next vertex of current triangle

		if (vertexInd == 3) //next triangle
		{
			vertexInd = 0;
			triangleCnt++;
		}
	}

	return indices;
}

bool ADestroyPack::IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices, DelaBella_Vertex v, std::vector<int> indices, int & oldIndex)
{
	for (int i = 0; i < triangleVertices.size(); i++)
	{

		if (v.x == triangleVertices[i].x && v.y == triangleVertices[i].y)
		{
			oldIndex = indices[i];
			return true;
		}

	}

	return false;
}

void ADestroyPack::CreateTriangle(int i)
{
	TArray<FVector> triVerts;
	TArray<int> triIndices;
	TArray<FVector> _normals;
	TArray<FVector2D> _UV0;
	TArray<FProcMeshTangent> _tangents;
	TArray<FLinearColor> _vertexColors;

	for (int32 v = 0; v < 3; v++) {
		triVerts.Add(Vertices[Triangles[i + v]]);
		triIndices.Add(v);
		_UV0.Add(UV0[Triangles[i + v]]);
		_tangents.Add(FProcMeshTangent(0, 1, 0));
		_vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
		_normals.Add(FVector(1, 0, 0));
	}
	//UE_LOG(LogTemp, Warning, TEXT("(v %d): (%s) (v %d): (%s) (v %d): (%s) "),triIndices[0],*triVerts[0].ToString(), triIndices[1], *triVerts[1].ToString(), triIndices[2], *triVerts[2].ToString())
	
	Mesh->CreateMeshSection_LinearColor(triangleInd, triVerts, triIndices, _normals, _UV0, _vertexColors, _tangents, true);
	triangleInd++;
}
