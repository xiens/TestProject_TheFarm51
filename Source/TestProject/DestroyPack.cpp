// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyPack.h"
#include "Components/StaticMeshComponent.h"
#include "Public\Point2.h"
#include <vector>
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreUObject/Public/UObject/Class.h"
#include "UnrealNetwork.h"
#include "Public/MeshGenerator.h"
#include "Components/BoxComponent.h"
#include "TestProjectCharacter.h"
#include "Runtime/Engine/Public/TimerManager.h"

// Sets default values
ADestroyPack::ADestroyPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComponent = Mesh;
	
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	Mesh->bUseAsyncCooking = true;
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetNotifyRigidBodyCollision(true);


	MeshGen = new MeshGenerator();
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
	MeshGen->GenerateMesh(Points, Height, Width, Mesh);
}

void ADestroyPack::PostLoad()
{
	Super::PostLoad();
}


void ADestroyPack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Cast<ATestProjectCharacter>(OtherActor) && DoOnce) {

		GetWorld()->GetTimerManager().SetTimer(CallMoveMeshSectionHandle, this, &ADestroyPack::HandleMoveSectionTimer, 0.003f, true);
		DoOnce = false;
	}
}
void ADestroyPack::HandleMoveSectionTimer() {

		FTimerDelegate TimerDel;
		FTimerHandle MoveMeshSectionHandle;

		//Binding the function with specific values
		TimerDel.BindUFunction(this, FName("MoveMeshSection"), SectionCnt, MoveBy);

		GetWorld()->GetTimerManager().SetTimer(MoveMeshSectionHandle, TimerDel, 0.02f, true);
		SectionCnt++;

		if (SectionCnt == Mesh->GetNumSections()) {
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
			GetWorld()->GetTimerManager().SetTimer(CallDestroyQuadSectionsHandle, this, &ADestroyPack::HideQuadSections, 0.01f, true, 1.0f);
		}

}

void ADestroyPack::MoveMeshSection(int32 SectionIndex, FVector MoveBy) {

	TArray <FProcMeshVertex> ProcVertexBuffer = Mesh->GetProcMeshSection(SectionIndex)->ProcVertexBuffer;

	TArray<FVector> Positions;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	for (size_t vert = 0; vert < 3; vert++)
	{
		FColor Color = ProcVertexBuffer[vert].Color;
		FVector Normal = ProcVertexBuffer[vert].Normal;
		FVector Position = ProcVertexBuffer[vert].Position;
		FProcMeshTangent Tangent = ProcVertexBuffer[vert].Tangent;
		FVector2D UV = ProcVertexBuffer[vert].UV0;

		Positions.Add(FVector(Position + MoveBy));
		Normals.Add(Normal);
		UVs.Add(UV);
		Colors.Add(Color);
		Tangents.Add(Tangent);
	}

	Mesh->UpdateMeshSection(SectionIndex, Positions, Normals, UVs, Colors, Tangents);
}
void ADestroyPack::HideQuadSections()
{
	if (Mesh) {
		CurrentHealth -= DmgAmount;

		temp += DmgAmount;

		int32 i = FMath::RandRange(0, Mesh->GetNumSections() - 1);

		if (temp >= HideSectionAmount) {
			temp = 0;
			Mesh->SetMeshSectionVisible(i, false);
		}

		if (CurrentHealth < 0) {
			Mesh->DestroyComponent();
			CurrentHealth = MaxHealth;
			GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		}
	}

	//Server_DestroyQuadSections();

}

void ADestroyPack::HideRandomTriangles()
{
	temp += DmgAmount;

	int32 i = FMath::RandRange(0, Mesh->GetNumSections() - 1);

	if (temp >= HideSectionAmount) {
		temp = 0;
		bool IsVisible = FMath::RandRange(0.0f, 1.0f) > 0.5f ? true : false;
		Mesh->SetMeshSectionVisible(i, IsVisible);
	}
}

void ADestroyPack::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	delete MeshGen;
}

void ADestroyPack::Server_DestroyQuadSections_Implementation()
{
	CurrentHealth -= DmgAmount;
	temp += DmgAmount;
	int32 i = FMath::RandRange(1, Points - 1);

	if (temp >= HideSectionAmount) {
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



//void ADestroyPack::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//	//DOREPLIFETIME(ADestroyPack, Mesh);
//}

