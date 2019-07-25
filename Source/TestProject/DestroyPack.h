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

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	int Points = 200; //Number of points, used in triangulation
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	float Width = 100.0f; //Width of generated quad
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	float Height = 450.0f; //Height of generated quad
	UPROPERTY(EditAnywhere, Category = "Destruction parameters")
	float DmgAmount = 0.1f; //Damage caused by every player's hit
	UPROPERTY(EditAnywhere, Category = "Destruction parameters")
	float HideSectionAmount = 0.5f; //Amount of damage to hide another mesh section

	float MaxHealth = 100.0f, CurrentHealth = 100.0f; //Object's health in %
	float temp = 0; //Temporary variable used in HideQuadSections()

	UFUNCTION()
	void OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit);

	void PostActorCreated();

	void PostLoad();

	bool DoOnce = true; //Used to set the timer only once when collision with this object occurs
	int SectionCnt = 0; //Counter for sections
	FVector MoveBy = FVector(0.1f, 0.3f, 0); //Vector to translate the mesh section

	FTimerHandle CallMoveMeshSectionHandle, CallDestroyQuadSectionsHandle;

	/**
	* Hides some random sections of quad
	* when CurrentHealth gets to 0, destroys the quad
	*/
	UFUNCTION()
	void HideQuadSections();

	/**
	* Moves the section of the mesh by @MoveBy vector
	* @MoveBy - FVector to translate the 3 vertices of triangle
	* @SectionIndex - index of mesh's section
	*/
	UFUNCTION()
	void MoveMeshSection(int32 SectionIndex, FVector MoveBy);

	/**
	* Sets the timers to move sections and stops them when 
	* all timers for moving sections were fired off
	* then starts the timer for HideQuadSections()
	*/
	UFUNCTION()
	void HandleMoveSectionTimer();

	void HideRandomTriangles();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DestroyQuadSections();

};
