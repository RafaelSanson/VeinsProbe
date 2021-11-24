// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph.h"
#include "Engine/StaticMeshActor.h"
#include "PathActor.generated.h"


/**
 *
 */
UCLASS(Blueprintable)
class VEINSPROBE_API APathActor : public AActor
{
	GENERATED_BODY()

public:
	APathActor();

	TSet<FVector> checkedLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Voxelization")
		float VoxelWidth = 10.0f;


	UFUNCTION(BlueprintCallable, Category = "Voxelization")
		void VoxelizeTrace(FVector startLocation, bool bShouldDisplayDebugInfo);

	UFUNCTION(BlueprintCallable, Category = "Voxelization")
		bool LineTraceTest(FVector startLocation, FVector endLocation, bool bShouldDisplayDebugInfo);

	UFUNCTION(BlueprintCallable, Category = "Graph")
		void GetMinimalPath(FVector origin, FVector destination, UPARAM(ref) TArray<FVector> &path);

	UFUNCTION(BlueprintCallable, Category = "Graph")
		FVector SnapToCheckedLocations(FVector endLocation);

	UFUNCTION(BlueprintCallable, Category = "Graph")
		FVector FindAlignedLocation(FVector baseLocation, float maxDistance);
	
	UFUNCTION(BlueprintCallable, Category = "Graph")
		void ClearCheckedLocations();

	void GetNextLocations(TArray<FVector> &nextLocations, FVector &startLocation);
};
