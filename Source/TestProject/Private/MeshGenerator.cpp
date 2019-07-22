// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"
#include "Public\Point2.h"
#include <vector>

MeshGenerator::MeshGenerator()
{
}

MeshGenerator::~MeshGenerator()
{
}

void MeshGenerator::GenerateMesh(int32 Points, float Height, float Width, UProceduralMeshComponent * Mesh)
{
	CreateQuad(Points, Height, Width, Mesh);
}

void MeshGenerator::CreateQuad(int32 Points, float Height, float Width, UProceduralMeshComponent * Mesh)
{
	Point2* cloud = new Point2[Points];

	if (Points < 5) return;

	//corners of quad
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

	UV0.AddZeroed(triangleVertices.size());

	for (int i = 0; i < triangleVertices.size(); i++)
	{
		Normals.Add(FVector(0, 0, 1));
		Vertices.Add(FVector(triangleVertices[i].x, triangleVertices[i].y, 0));
		Tangents.Add(FProcMeshTangent(0, 1, 0));
		VertexColors.Add(FLinearColor(1, 0, 0, 1.0));
		UV0[i] = CalculateUV(Vertices[i], Height, Width);
	}

	//Create section for every triangle in our mesh
	for (int i = 0; i < triangleIndices.size() / 3; i++) {
		CreateTriangle(i, Mesh);
	}

	// Enable collision data
	Mesh->ContainsPhysicsTriMeshData(true);
}

FVector2D MeshGenerator::CalculateUV(const FVector &Vertices, float Height, float Width)
{
	return FVector2D(1 - (Vertices.Y + Height/2.0f) / Height, (Vertices.X + Width/2.0f) / Width);
}
std::vector<int> MeshGenerator::CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex>& triangleVertices)
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

bool MeshGenerator::IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices, DelaBella_Vertex v, std::vector<int> indices, int & oldIndex)
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

void MeshGenerator::CreateTriangle(int i, UProceduralMeshComponent * Mesh)
{
	TArray<FVector> triVerts;
	TArray<int> triIndices;
	TArray<FVector> _normals;
	TArray<FVector2D> _UV0;
	TArray<FProcMeshTangent> _tangents;
	TArray<FLinearColor> _vertexColors;

	for (int32 v = 0; v < 3; v++) {
		triVerts.Add(Vertices[Triangles[3 * i + v]]);
		triIndices.Add(v);
		_UV0.Add(UV0[Triangles[3 * i + v]]);
		_tangents.Add(FProcMeshTangent(0, 1, 0));
		_vertexColors.Add(FLinearColor(1, 0, 0, 1.0));
		_normals.Add(FVector(0, 0, 1));
	}

	Mesh->CreateMeshSection_LinearColor(i, triVerts, triIndices, _normals, _UV0, _vertexColors, _tangents, true);
}
