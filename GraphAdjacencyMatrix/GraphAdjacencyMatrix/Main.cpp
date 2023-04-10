// Christian Hinkle


#include "CTHGraphAdjacencyMatrix.h"
#include <climits>
#include <iostream>
//#include "../CTHStringFunctionLibrary.h"
#include <string>
#include <fstream>
#include <sstream>
#include "CTHGraphAdjacencyMatrixFunctionLibrary.h"



const std::string InputFileName = std::string("Input.txt");

const int ExitCodeErrorNoFileInput = 1;

template <class T, class TWeight, TWeight WeightInvalid>
void BuildGraphFromInputStream(std::istream& InOutInputStream, CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>& OutGraph);

template <class T, class TWeight, TWeight WeightInvalid>
void PrintShortestPaths(std::ostream& InOutOutputStream, const std::map<T, std::vector<std::pair<T, TWeight>>>& InShortestPaths);

void Testing();

/**
 * Entry point of application
 */
int main()
{
	std::ifstream InputFileStream = std::ifstream(InputFileName);
	if (InputFileStream.is_open() == false)
	{
		std::cout << "No \"" << InputFileName << "\" file present. Exiting." << '\n';
		return ExitCodeErrorNoFileInput;
	}

	std::cout << "Reading \"" << InputFileName << "\" and building graph." << '\n';
	CTHGraphAdjacencyMatrix<char, int, -1> Graph;
	BuildGraphFromInputStream(InputFileStream, Graph);

	std::cout << "Graph adjacency matrix:" << '\n';
	Graph.PrintAdjacencyMatrix(std::cout);

	std::cout << '\n';

	const char VertexFrom = 'd';
	const std::map<char, std::vector<std::pair<char, int>>> ShortestPaths = CTHGraphAdjacencyMatrixFunctionLibrary::DijkstraShortestPaths(Graph, VertexFrom);

	std::cout << "Dijkstra shortest paths starting at vertex '" << VertexFrom << "':" << '\n';
	PrintShortestPaths<char, int, -1>(std::cout, ShortestPaths);


	return 0; // exit with success status
}

template <class T, class TWeight, TWeight WeightInvalid>
void BuildGraphFromInputStream(std::istream& InOutInputStream, CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>& OutGraph)
{
	std::string Line;
	while (std::getline(InOutInputStream, Line))
	{
		std::stringstream LineStream = std::stringstream(Line);

		T VertexFrom;
		{
			std::string VertexFromString;
			std::getline(LineStream, VertexFromString, ',');
			std::stringstream VertexFromStringStream = std::stringstream(VertexFromString);
			VertexFromStringStream >> VertexFrom;
		}

		T VertexTo;
		{
			std::string VertexToString;
			std::getline(LineStream, VertexToString, ',');
			std::stringstream VertexToStringStream = std::stringstream(VertexToString);
			VertexToStringStream >> VertexTo;
		}

		TWeight EdgeWeight;
		{
			std::string EdgeWeightString;
			std::getline(LineStream, EdgeWeightString, ',');
			std::stringstream EdgeWeightStringStream = std::stringstream(EdgeWeightString);
			EdgeWeightStringStream >> EdgeWeight;
		}

		if (OutGraph.HasVertex(VertexFrom) == false)
		{
			OutGraph.AddVertex(VertexFrom);
		}
		if (OutGraph.HasVertex(VertexTo) == false)
		{
			OutGraph.AddVertex(VertexTo);
		}

		OutGraph.AddEdge(EdgeWeight, VertexFrom, VertexTo);
	}
}

template<class T, class TWeight, TWeight WeightInvalid>
void PrintShortestPaths(std::ostream& InOutOutputStream, const std::map<T, std::vector<std::pair<T, TWeight>>>& InShortestPaths)
{
	for (typename std::map<T, std::vector<std::pair<T, TWeight>>>::const_iterator PathIterator = InShortestPaths.begin(); PathIterator != InShortestPaths.end(); ++PathIterator)
	{
		// NOTE: we currently enclose the vertex value in "'" assuming that it is a char type - this is weird
		const TWeight PathWeight = CTHGraphAdjacencyMatrixFunctionLibrary::GetPathWeight<char, int, -1>(PathIterator->second);
		std::cout << "Path to '" << PathIterator->first << "' has weight of " << PathWeight << ":    ";

		for (typename std::vector<std::pair<T, TWeight>>::const_iterator VertexIterator = PathIterator->second.begin(); VertexIterator != PathIterator->second.end(); ++VertexIterator)
		{
			if (VertexIterator != PathIterator->second.begin())
			{
				std::cout << " --" << VertexIterator->second << "--> ";
			}

			std::cout << '\'' << VertexIterator->first << '\'';
		}

		std::cout << '\n';
	}
}

void Testing()
{
	CTHGraphAdjacencyMatrix<char, unsigned int, UINT_MAX> Graph;
	Graph.AddVertex('a');
	Graph.AddVertex('b');
	Graph.AddEdge(5, 'a', 'b');
	Graph.AddVertex('e');
	Graph.AddVertex('t');
	Graph.AddEdge(2, 'a', 't');
	Graph.AddEdge(7, 't', 'e');

	Graph.PrintAdjacencyMatrix(std::cout);
	std::cout << '\n';

	Graph.RemoveVertex('e');

	Graph.PrintAdjacencyMatrix(std::cout);
	std::cout << '\n';

	Graph.RemoveEdge('a', 't');

	Graph.PrintAdjacencyMatrix(std::cout);
	std::cout << '\n';

	Graph.AddEdge(3, 'a', 't');
	Graph.AddEdge(9, 'b', 't');
	Graph.AddVertex('o');
	Graph.AddEdge(3, 'o', 'a');
	Graph.AddEdge(8, 'o', 'b');
	Graph.AddEdge(5, 'a', 'o');
	Graph.AddEdge(1, 't', 'o');

	Graph.PrintAdjacencyMatrix(std::cout);
	std::cout << '\n';

	//const std::vector<char> PathAToT = CTHGraphAdjacencyMatrixFunctionLibrary::DijkstraShortestPath(Graph, 'a', 't');
	//std::cout << "Shortest path from 'a' to 't': " << CTHStringFunctionLibrary::ContainerToString(PathAToT.begin(), PathAToT.end()) << '\n';

	CTHGraphAdjacencyMatrixFunctionLibrary::DepthFirstTraversal<char, unsigned int, UINT_MAX>(Graph, 'a',
		[](const char& InVertex) -> void
		{
			std::cout << InVertex << ' ';
		}
	);
}
