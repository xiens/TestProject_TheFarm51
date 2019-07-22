// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "delabella.h"
#include <vector>
#include "ProceduralMeshComponent.h"

class Point2;
/**
 * Generates rectangular mesh, consisting of triangles, using Delaunay Triangulation
 */
class TESTPROJECT_API MeshGenerator
{
public:
	MeshGenerator();
	~MeshGenerator();

	/**
	 * Generates rectangular mesh for given number of @Points with specified @Width and @Height
	 * @param Points - number of points to generate mesh
	 * @param Height - height of rectangle
	 * @param Width - width of rectangle
	 * @param Mesh - mesh to modify
	 */
	void GenerateMesh(int32 Points, float Height, float Width, UProceduralMeshComponent * Mesh);

private:

	//Mesh components
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	//Creates the quad from triangles using Delaunay Triangulation
	void CreateQuad(int32 Points, float Height, float Width, UProceduralMeshComponent * Mesh);

	FVector2D CalculateUV(const FVector &Vertices, float Height, float Width);

	//Returns the triangle indices needed for mesh creation
	std::vector<int> CalculateTriangleIndices(std::vector<DelaBella_Triangle> triangles, std::vector<DelaBella_Vertex> &triangleVertices);

	//Checks if the vertex v is in the triangleVertices vector, also returns vertex's index
	bool IsVertexDefined(std::vector<DelaBella_Vertex> triangleVertices, DelaBella_Vertex v, std::vector<int>indices, int &oldIndex);

	//Adds one triangle section to mesh
	void CreateTriangle(int i, UProceduralMeshComponent * Mesh);
};
