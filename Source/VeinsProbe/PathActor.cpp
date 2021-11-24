// Fill out your copyright notice in the Description page of Project Settings.


#include "PathActor.h"
#include "DrawDebugHelpers.h"


APathActor::APathActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APathActor::VoxelizeTrace(FVector startLocation, bool bShouldDisplayDebugInfo)
{
	if (!checkedLocations.Contains(startLocation))
	{
		checkedLocations.Add(startLocation);
	}
	
	TArray<FVector> nextLocations;
	GetNextLocations(nextLocations, startLocation);

	for (auto& nextLocation : nextLocations)
	{
		if (!checkedLocations.Contains(nextLocation))
		{
			if (!LineTraceTest(startLocation, nextLocation, bShouldDisplayDebugInfo))
			{
				if (bShouldDisplayDebugInfo)
				{
					DrawDebugSphere(this->GetWorld(), nextLocation, 10.f, 4, FColor::White, true, 10.f, 0, 1.f);
				}

				VoxelizeTrace(nextLocation, bShouldDisplayDebugInfo);
			}
		}
	}

	nextLocations.Empty();
}

bool APathActor::LineTraceTest(FVector startLocation, FVector endLocation, bool bShouldDisplayDebugInfo)
{
	FCollisionQueryParams traceParams = FCollisionQueryParams();
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	if (bShouldDisplayDebugInfo)
	{
		const FName TraceTag("MyTraceTag");
		GetWorld()->DebugDrawTraceTag = TraceTag;
		traceParams.TraceTag = TraceTag;
	}

	//Re-initialize hit info
	FHitResult hitOutput(ForceInit);

	//call GetWorld() from within an actor extending class
	GetWorld()->LineTraceSingleByChannel(
		hitOutput,        //result
		startLocation,    //start
		endLocation, //end
		ECC_Visibility, //collision channel
		traceParams
	);

	return hitOutput.bBlockingHit;
}

void APathActor::GetNextLocations(TArray<FVector> &nextLocations, FVector &startLocation)
{
	nextLocations.Add(startLocation + FVector::UpVector * VoxelWidth);
	nextLocations.Add(startLocation + FVector::DownVector * VoxelWidth);
	nextLocations.Add(startLocation + FVector::RightVector * VoxelWidth);
	nextLocations.Add(startLocation + FVector::LeftVector * VoxelWidth);
	nextLocations.Add(startLocation + FVector::ForwardVector * VoxelWidth);
	nextLocations.Add(startLocation + FVector::BackwardVector * VoxelWidth);
}

FVector APathActor::SnapToCheckedLocations(FVector endLocation)
{
	if(checkedLocations.Contains(endLocation))
	{
		return endLocation;
	}
	else
	{
		for (auto& currentLocation : checkedLocations)
		{
			if (FVector::Dist(currentLocation, endLocation) < VoxelWidth)
				return currentLocation;
		}
	}

	return FVector();
}


FVector APathActor::FindAlignedLocation(FVector baseLocation, float maxDistance)
{
	FVector alignedLocation = FVector::ZeroVector;
	int checkedLocationsCount = 0;
	for (auto& currentLocation : checkedLocations)
	{
		if (FVector::Dist(baseLocation, currentLocation) < maxDistance)
		{
			alignedLocation += currentLocation;
			checkedLocationsCount++;
		}
	}
	alignedLocation /= (float) checkedLocationsCount;
	return alignedLocation;
}

void APathActor::ClearCheckedLocations()
{
	checkedLocations.Empty();
}



void APathActor::GetMinimalPath(FVector origin, FVector destination, UPARAM(ref) TArray<FVector> &path)
{
	Graph g;

	path.Empty();

	destination = SnapToCheckedLocations(destination);


	// Populate graph with vertex from TSet<FVector> checkedLocations
	for (auto& currentLocation : checkedLocations)
	{
		g.AddVertex(currentLocation.ToString(), false, 0.0f, &currentLocation);
	}

	// Populate graph with edges based on TSet<FVector> checkedLocations
	for (auto& currentLocation : checkedLocations)
	{
		TArray<FVector> nextLocations;
		GetNextLocations(nextLocations, currentLocation);


		for (FVector nextLocation : nextLocations)
		{
			if (checkedLocations.Contains(nextLocation))
			{
				g.AddEdge(currentLocation.ToString(), nextLocation.ToString(), VoxelWidth, false);
			}
		}
	}

	Graph r;
	g.Dijkstra(&r, origin.ToString(), destination.ToString());

	r.GetPath(origin.ToString(), &path);
}