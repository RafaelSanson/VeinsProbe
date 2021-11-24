// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
#include <map>
#include <string>
#include <queue>
#include <limits>
using namespace std;

struct Vertex;
struct Edge;

struct Vertex {
	FString Name;
	bool Visited;
	float PathLength;
	FVector* position;
	map<FString, Edge> edges;
};

struct Edge {
	Vertex* destination;
	float length;
};


/**
 * 
 */
class VEINSPROBE_API Graph
{
public:
	Graph();
	~Graph();

	map<FString, Vertex> vertexes;

	void AddVertex(FString name, bool visited, float pathLength, FVector* position);
	void AddCloneVertex(Vertex* newVertex);
	Vertex* FindVertex(FString name);
	void AddEdge(FString originName, FString destinationName, float distance = 1.0f, bool bidirectional = false);
	void Dijkstra(Graph* pathTree, FString originName, FString destinationName);
	void GetPath(FString origin, TArray<FVector>* path);
	Edge* GetFirstEdge(Vertex* currentVertex);
};
