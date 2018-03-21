#pragma once
#include <unordered_map>
#include <memory>
#include "GridCell.h"
#include "Range3D.h"

namespace SparseGrid
{
	class CubeSparseGrid
	{
	public:
		typedef unsigned long long CellIndex;
		typedef unsigned long long CellPosition;

		CubeSparseGrid(Range3D const& boundingBox, float cellSize) : _BoundingBox{boundingBox}, _CellSize{cellSize}
		{
			double xDistance = boundingBox.Max.X - boundingBox.Min.X;
			double yDistance = boundingBox.Max.Y - boundingBox.Min.Y;
			double zDistance = boundingBox.Max.Z - boundingBox.Min.Z;

			_TotalCellsX = ceil(xDistance / cellSize);
			_TotalCellsY = ceil(yDistance / cellSize);
			_TotalCellsZ = ceil(zDistance / cellSize);

			_TotalCellsXY = _TotalCellsX * _TotalCellsY;
			_TotalCellsXYZ = _TotalCellsXY * _TotalCellsZ;
		}

		~CubeSparseGrid() = default;

		bool GetCellIndex(CellPosition x, CellPosition y, CellPosition z, CellIndex* cellIndex)
		{
			if (x >= _TotalCellsX || y >= _TotalCellsY || z >= _TotalCellsZ)
				return false;

			CellIndex depth = z * _TotalCellsXY;
			*cellIndex = y * _TotalCellsX + x + depth;

			return true;
		}

		bool GetCellIndex(Point3D point, CellIndex* cellIndex) 
		{
			if (point.X > _BoundingBox.Max.X || point.X < _BoundingBox.Min.X || point.Y > _BoundingBox.Max.Y || point.Y < _BoundingBox.Min.Y || point.Z > _BoundingBox.Max.Z || point.Z < _BoundingBox.Min.Z)
				return false;

			double xDistance = point.X - _BoundingBox.Min.X;
			double yDistance = point.Y - _BoundingBox.Min.Y;
			double zDistance = point.Z - _BoundingBox.Min.Z;

			CellPosition x = ceil(xDistance / _CellSize);
			CellPosition y = ceil(yDistance / _CellSize);
			CellPosition z = ceil(zDistance / _CellSize);

			return GetCellIndex(x, y, z, cellIndex);
		}

		bool InsertData(CellIndex cellIndex, std::shared_ptr<GridCell> &cell)
		{
			auto iterator = _CellMap.find(cellIndex);
			if (iterator != _CellMap.end())
				return false;
			
			_CellMap[cellIndex] = std::move(cell);
			return true;
		}

		bool GetData(CellIndex cellIndex, std::shared_ptr<GridCell> *cell)
		{
			auto iterator = _CellMap.find(cellIndex);
			if (iterator == _CellMap.end())
				return false;

			*cell = iterator->second;
			return true;
		}

	private:
		std::unordered_map<CellIndex, std::shared_ptr<GridCell>> _CellMap;
		Range3D _BoundingBox;
		float _CellSize;

		CellIndex _TotalCellsX, _TotalCellsY, _TotalCellsZ;
		CellIndex _TotalCellsXY;
		CellIndex _TotalCellsXYZ;
	};


}


