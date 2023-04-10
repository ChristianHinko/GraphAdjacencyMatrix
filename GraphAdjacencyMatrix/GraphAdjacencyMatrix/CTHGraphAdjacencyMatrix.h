// Christian Hinkle

#pragma once

#include <ostream>
#include <map>



/**
 * A graph implemented via an adjacent matrix
 */
template <class T, class TWeight, TWeight WeightInvalid>
class CTHGraphAdjacencyMatrix
{
	friend class CTHGraphAdjacencyMatrixFunctionLibrary; // weird
public:
	/** Add a vertex with a value to the graph. */
	void AddVertex(const T& InValue);

	/** Remove a vertex by value from the graph. */
	void RemoveVertex(const T& InValue);

	/** Add an edge (with a weight) from one vertex to another. */
	void AddEdge(const TWeight& InWeight, const T& InValueFrom, const T& InValueTo);

	/** Remove an edge from the graph. */
	void RemoveEdge(const T& InValueFrom, const T& InValueTo);

	/** Output the adjacency matrix to a given stream. */
	void PrintAdjacencyMatrix(std::ostream& InOutOutputStream) const;

	/** Get the number of vertices in the graph. */
	std::size_t GetNumVertices() const;

	bool HasVertex(const T& InValue) const;

protected:
	/** The adjacency matrix holding weights from vertex to vertex */
	std::map<T, std::map<T, TWeight>> WeightMatrix;
};


template <class T, class TWeight, TWeight WeightInvalid>
void CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::AddVertex(const T& InValue)
{
	// Expand the matrix for a new vertex
	std::map<T, TWeight> NewColumn;
	for (const std::pair<T, std::map<T, TWeight>>& Column : WeightMatrix)
	{
		NewColumn.emplace(Column.first, WeightInvalid);
	}

	WeightMatrix.emplace(InValue, NewColumn); // make a new column
	for (typename std::map<T, std::map<T, TWeight>>::iterator ColumnIterator = WeightMatrix.begin(); ColumnIterator != WeightMatrix.end(); ++ColumnIterator)
	{
		ColumnIterator->second.emplace(InValue, WeightInvalid);
	}

	// Initialize this vertex weight to default value
	WeightMatrix[InValue][InValue] = TWeight();
}

template <class T, class TWeight, TWeight WeightInvalid>
void CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::RemoveVertex(const T& InValue)
{
	// Remove the Row of this vertex's from points
	WeightMatrix.erase(InValue);

	// Remove the Column of this vertex's to points
	for (typename std::map<T, std::map<T, TWeight>>::iterator ColumnIterator = WeightMatrix.begin(); ColumnIterator != WeightMatrix.end(); ++ColumnIterator)
	{
		ColumnIterator->second.erase(InValue);
	}
}

template <class T, class TWeight, TWeight WeightInvalid>
void CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::AddEdge(const TWeight& InWeight, const T& InValueFrom, const T& InValueTo)
{
	WeightMatrix[InValueFrom][InValueTo] = InWeight;
}

template <class T, class TWeight, TWeight WeightInvalid>
void CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::RemoveEdge(const T& InValueFrom, const T& InValueTo)
{
	WeightMatrix[InValueFrom][InValueTo] = WeightInvalid;
}

template <class T, class TWeight, TWeight WeightInvalid>
void CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::PrintAdjacencyMatrix(std::ostream& InOutOutputStream) const
{
	// Print header row
	InOutOutputStream << "   ";
	for (typename std::map<T, std::map<T, TWeight>>::const_iterator ColumnIterator = WeightMatrix.begin(); ColumnIterator != WeightMatrix.end(); ++ColumnIterator)
	{
		InOutOutputStream << ColumnIterator->first;

		if (std::next(ColumnIterator) != WeightMatrix.end())
		{
			InOutOutputStream << ' ';
		}
	}

	InOutOutputStream << '\n';
	InOutOutputStream << '\n';

	for (typename std::map<T, std::map<T, TWeight>>::const_iterator ColumnIterator = WeightMatrix.begin(); ColumnIterator != WeightMatrix.end(); ++ColumnIterator)
	{
		InOutOutputStream << ColumnIterator->first;
		InOutOutputStream << "  ";

		for (typename std::map<T, TWeight>::const_iterator ItemIterator = ColumnIterator->second.begin(); ItemIterator != ColumnIterator->second.end(); ++ItemIterator)
		{
			const TWeight& Weight = ItemIterator->second;
			if (Weight == WeightInvalid)
			{
				InOutOutputStream << '-';
			}
			else
			{
				InOutOutputStream << Weight;
			}

			if (std::next(ItemIterator) != ColumnIterator->second.end())
			{
				InOutOutputStream << " ";
			}
		}

		InOutOutputStream << '\n';
	}
}

template <class T, class TWeight, TWeight WeightInvalid>
std::size_t CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::GetNumVertices() const
{
	return WeightMatrix.size();
}

template <class T, class TWeight, TWeight WeightInvalid>
bool CTHGraphAdjacencyMatrix<T, TWeight, WeightInvalid>::HasVertex(const T& InValue) const
{
	return (WeightMatrix.count(InValue) > 0);
}
