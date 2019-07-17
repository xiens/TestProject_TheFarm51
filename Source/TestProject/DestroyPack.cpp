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

// Sets default values
ADestroyPack::ADestroyPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxComponent->SetBoxExtent(FVector(Width*0.5f, Height*0.5f, 1.0f));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BoxComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetSimulatePhysics(true);

	RootComponent = BoxComponent;

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	Mesh->bUseAsyncCooking = true;
	Mesh->SetSimulatePhysics(true);

	Mesh->SetupAttachment(RootComponent);
	
	MeshGen = new MeshGenerator();
}

// Called when the game starts or when spawned
void ADestroyPack::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentHit.AddDynamic(this, &ADestroyPack::OnHit);
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
	if (Cast<ATestProjectCharacter>(OtherActor)) {
		DestroyQuadSections();
	}
}

void ADestroyPack::DestroyQuadSections()
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
		BoxComponent->DestroyComponent();
		CurrentHealth = 100.0f;
	}
	Server_DestroyQuadSections();
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

