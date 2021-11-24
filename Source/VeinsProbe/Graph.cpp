// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::AddVertex(FString name, bool visited, float pathLength, FVector* position)
{
	if (FindVertex(name) == nullptr)
	{
		vertexes[name] = { name, false, 0, position };
	}
}

void Graph::AddCloneVertex(Vertex* newVertex)
{
	if (FindVertex(newVertex->Name) == nullptr)
	{
		vertexes[newVertex->Name] = { newVertex->Name, newVertex->Visited, newVertex->PathLength, newVertex->position };
	}
}

Vertex * Graph::FindVertex(FString name)
{
	map<FString, Vertex>::iterator it;
	it = vertexes.find(name);
	if (it != vertexes.end())
	{
		return &it->second;
	}

	return nullptr;
}

void Graph::AddEdge(FString originName, FString destinationName, float distance, bool bidirectional)
{
	Vertex* origin = FindVertex(originName);
	Vertex* destination = FindVertex(destinationName);

	if (origin == nullptr) return;
	if (destination == nullptr) return;

	Edge edge = { destination, distance };
	origin->edges[destinationName] = edge;

	if (bidirectional)
	{
		Edge backEdge = { origin, distance };
		destination->edges[originName] = backEdge;
	}
}

void Graph::Dijkstra(Graph* shortestPath, FString originName, FString destinationName)
{
	if (originName == destinationName) return;

	float floatMaxValue = numeric_limits<float>::max();

	Vertex* origin = FindVertex(originName);
	Vertex* destination = FindVertex(destinationName);
	if (origin == nullptr) return;
	if (destination == nullptr) return;

	Graph pathTree;


	for (map<FString, Vertex>::iterator it = vertexes.begin(); it != vertexes.end(); ++it)
	{
		it->second.PathLength = floatMaxValue;
		Vertex v = { it->second.Name, false, 0.f, it->second.position };
		pathTree.AddCloneVertex(&v);
	}


	queue<Vertex*> vertexQueue;
	map<FString, Edge>::iterator edgeIterator;
	Edge* currentEdge;
	Vertex* currentVertex;
	Vertex* nextVertex;
	float accumulatedPathLength;

	origin->PathLength = 0.f;
	vertexQueue.push(origin);
	do
	{
		currentVertex = vertexQueue.front();
		vertexQueue.pop();

		for (edgeIterator = currentVertex->edges.begin(); edgeIterator != currentVertex->edges.end(); ++edgeIterator)
		{
			currentEdge = &edgeIterator->second;
			if (currentEdge != nullptr)
			{
				accumulatedPathLength = currentEdge->length + currentVertex->PathLength;
				nextVertex = &vertexes[currentEdge->destination->Name];
				if (!nextVertex->Visited && accumulatedPathLength < nextVertex->PathLength)
				{
					nextVertex->PathLength = accumulatedPathLength;
					pathTree.AddEdge(nextVertex->Name, currentVertex->Name);
					vertexQueue.push(nextVertex);
				}
			}
		}

		currentVertex->Visited = true;
	} while (vertexQueue.size() > 0);


	if (!destination->Visited)
	{
		return;
	}

	currentVertex = &pathTree.vertexes[destination->Name];
	shortestPath->AddVertex(currentVertex->Name, currentVertex->Visited, currentVertex->PathLength, currentVertex->position);

	while (true)
	{
		nextVertex = GetFirstEdge(currentVertex)->destination;
		if (nextVertex == nullptr) return;
		shortestPath->AddVertex(nextVertex->Name, nextVertex->Visited, nextVertex->PathLength, nextVertex->position);
		shortestPath->AddEdge(nextVertex->Name, currentVertex->Name, 0.0f, false);

		if (nextVertex->Name == origin->Name)
			return;

		if (nextVertex->Name == currentVertex->Name)
			return;
		currentVertex = nextVertex;
	}

	return;
}

void Graph::GetPath(FString origin, TArray<FVector>* path)
{
	if (path == nullptr) return;
	int maximumPathSize = vertexes.size();

	path->Empty(maximumPathSize);

	Vertex* currentVertex = FindVertex(origin);
	Edge* currentEdge;
	while (currentVertex != nullptr && maximumPathSize > 0)
	{
		if (currentVertex->position == nullptr) return;
		path->Add(FVector(*currentVertex->position));
		currentEdge = GetFirstEdge(currentVertex);

		if (currentEdge != nullptr)
		{
			currentVertex = currentEdge->destination;
		}
		else
		{
			currentVertex = nullptr;
		}

		maximumPathSize--;
	}
}

Edge* Graph::GetFirstEdge(Vertex * currentVertex)
{
	return &currentVertex->edges.begin()->second;
}
