// Christian Hinkle

#pragma once

#include "CTHGraphAdjacencyMatrix.h"
#include <vector>
#include <queue>
#include <map>
#include <functional>
#include <set>
#include <stack>



/**
 * A collection of helpful graph-related functions
 */
class CTHGraphAdjacencyMatrixFunctionLibrary
{
public:
	/**
	 * Get all shortest paths from a given vertex to the rest.
	 * 
	 * A path is an array of edges proceeded by the starting vertex.
	 */
	template <class T, class TWeight, TWeight WeightInvalid>
	static std::map<T, std::vector<std::pair<T, TWeight>>> DijkstraShortestPaths(const CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>& InGraph, const T& InVertexStart);

	/** Calculate the total weight of a given path */
	template <class T, class TWeight, TWeight WeightInvalid>
	static TWeight GetPathWeight(const std::vector<std::pair<T, TWeight>>& InPath);

	/** Get the path that has the least weight out of the given paths */
	template <class T, class TWeight, TWeight WeightInvalid>
	static std::vector<std::pair<T, TWeight>> GetShortestPath(const std::set<std::vector<std::pair<T, TWeight>>>& InPaths);



	/** Depth first traversal */
	template <class T, class TWeight, TWeight WeightInvalid>
	static void DepthFirstTraversal(const CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>& InGraph, const T& InVertexStart, const std::function<void(const T&)> InFunction);
};


template <class T, class TWeight, TWeight WeightInvalid>
std::map<T, std::vector<std::pair<T, TWeight>>> CTHGraphAdjacencyMatrixFunctionLibrary::DijkstraShortestPaths(const CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>& InGraph, const T& InVertexStart)
{
	// Map from destination vertices to the paths to lead to them
	std::map<T, std::vector<std::pair<T, TWeight>>> Paths;

	// Initialize with empty paths
	for (const std::pair<T, TWeight>& AdjacentEdge : InGraph.WeightMatrix.at(InVertexStart))
	{
		Paths.emplace(AdjacentEdge.first, std::vector<std::pair<T, TWeight>>());
	}

	// 
	{
		std::set<std::vector<std::pair<T, TWeight>>> QueuedPaths;
		QueuedPaths.emplace(std::vector<std::pair<T, TWeight>>({ std::pair<T, TWeight>(InVertexStart, TWeight()) }));
		while (QueuedPaths.empty() == false)
		{
			// Pop shortest path
			const std::vector<std::pair<T, TWeight>> ShortestPath = GetShortestPath<T, TWeight, WeightInvalid>(QueuedPaths); // copy it (instead of referencing it) so that it still exists after we erase it
			QueuedPaths.erase(ShortestPath);

			const T& CurrentVertex = ShortestPath.back().first;

			const std::map<T, TWeight> AdjacentEdges = InGraph.WeightMatrix.at(CurrentVertex);
			for (const std::pair<T, TWeight>& AdjacentEdge : AdjacentEdges)
			{
				if (AdjacentEdge.first == CurrentVertex)
				{
					// Ignore ourselves
					continue;
				}

				if (AdjacentEdge.second != WeightInvalid)
				{
					std::vector<std::pair<T, TWeight>> NewPath = ShortestPath;
					NewPath.push_back(AdjacentEdge);
					const TWeight PathWeight = GetPathWeight<T, TWeight, WeightInvalid>(NewPath);

					const std::vector<std::pair<T, TWeight>>& CurrentShortestPath = Paths[AdjacentEdge.first];
					const TWeight CurrentPathWeight = GetPathWeight<T, TWeight, WeightInvalid>(CurrentShortestPath);

					if (PathWeight <= CurrentPathWeight || CurrentPathWeight == WeightInvalid)
					{
						// Relax this path
						Paths[AdjacentEdge.first] = NewPath;

						// Queue this path for exploration later
						QueuedPaths.emplace(NewPath);
					}
				}
			}
		}
	}

	return Paths;
}

template <class T, class TWeight, TWeight WeightInvalid>
TWeight CTHGraphAdjacencyMatrixFunctionLibrary::GetPathWeight(const std::vector<std::pair<T, TWeight>>& InPath)
{
	if (InPath.empty())
	{
		return WeightInvalid;
	}

	TWeight PathWeight = TWeight();

	for (const std::pair<T, TWeight>& Edge : InPath)
	{
		PathWeight += Edge.second;
	}

	return PathWeight;
}

template <class T, class TWeight, TWeight WeightInvalid>
std::vector<std::pair<T, TWeight>> CTHGraphAdjacencyMatrixFunctionLibrary::GetShortestPath(const std::set<std::vector<std::pair<T, TWeight>>>& InPaths)
{
	const std::vector<std::pair<T, TWeight>>* ShortestPathPtr = nullptr;

	for (const std::vector<std::pair<T, TWeight>>& Path : InPaths)
	{
		if (ShortestPathPtr == nullptr)
		{
			ShortestPathPtr = &Path;
			continue;
		}

		const TWeight PathWeight = GetPathWeight<T, TWeight, WeightInvalid>(Path);
		const TWeight ShortestPathWeight = GetPathWeight<T, TWeight, WeightInvalid>(*ShortestPathPtr);
		if (ShortestPathWeight < PathWeight)
		{
			ShortestPathPtr = &Path;
		}
	}

	if (ShortestPathPtr == nullptr)
	{
		return std::vector<std::pair<T, TWeight>>();
	}

	return *ShortestPathPtr;
}

template <class T, class TWeight, TWeight WeightInvalid>
void CTHGraphAdjacencyMatrixFunctionLibrary::DepthFirstTraversal(const CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>& InGraph, const T& InVertexStart, const std::function<void(const T&)> InFunction)
{
	std::set<T> Visited;

	std::stack<T> CurrentDepth;
	CurrentDepth.push(InVertexStart);
	while (CurrentDepth.empty() == false)
	{
		const T CurrentVertex = CurrentDepth.top();
		CurrentDepth.pop();

		if (Visited.find(CurrentVertex) == Visited.end())
		{
			InFunction(CurrentVertex);
			Visited.emplace(CurrentVertex);
		}

		const std::map<T, TWeight> AdjacentVertices = InGraph.WeightMatrix.at(CurrentVertex);
		for (const std::pair<T, TWeight>& Edge : AdjacentVertices)
		{
			if (Edge.second != WeightInvalid)
			{
				if (Visited.find(Edge.first) == Visited.end())
				{
					CurrentDepth.push(Edge.first);
				}
			}
		}
	}
}
