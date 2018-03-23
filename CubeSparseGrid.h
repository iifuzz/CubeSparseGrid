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

		CubeSparseGrid(Range3D const& boundingBox, float cellSize, bool flexibleCellSize) : _BoundingBox{boundingBox}, _CellSizeX{cellSize}, _CellSizeY{ cellSize }, _CellSizeZ{ cellSize }
		{
			double xDistance = boundingBox.Max.X - boundingBox.Min.X;
			double yDistance = boundingBox.Max.Y - boundingBox.Min.Y;
			double zDistance = boundingBox.Max.Z - boundingBox.Min.Z;

			if (flexibleCellSize) {
				_CellSizeX = (float)(xDistance / std::round(xDistance / _CellSizeX));
				_CellSizeY = (float)(yDistance / std::round(yDistance / _CellSizeY));
				_CellSizeZ = (float)(zDistance / std::round(zDistance / _CellSizeZ));
			}


			_TotalCellsX = (CellIndex)ceil(xDistance / _CellSizeX);
			_TotalCellsY = (CellIndex)ceil(yDistance / _CellSizeY);
			_TotalCellsZ = (CellIndex)ceil(zDistance / _CellSizeZ);

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

			if (point.X == _BoundingBox.Max.X)
				point.X -= 0.000001;
			if (point.Y == _BoundingBox.Max.Y)
				point.Y -= 0.000001;
			if (point.Z == _BoundingBox.Max.Z)
				point.Z -= 0.000001;

			double xDistance = point.X - _BoundingBox.Min.X;
			double yDistance = point.Y - _BoundingBox.Min.Y;
			double zDistance = point.Z - _BoundingBox.Min.Z;

			CellPosition x = (CellIndex)floor(xDistance / _CellSizeX);
			CellPosition y = (CellIndex)floor(yDistance / _CellSizeY);
			CellPosition z = (CellIndex)floor(zDistance / _CellSizeZ);

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

		void GetCellSize(float *x, float *y, float *z) {
			*x = _CellSizeX;
			*y = _CellSizeY;
			*z = _CellSizeZ;
		}
	private:
		std::unordered_map<CellIndex, std::shared_ptr<GridCell>> _CellMap;
		Range3D _BoundingBox;
		float _CellSizeX, _CellSizeY, _CellSizeZ;

		CellIndex _TotalCellsX, _TotalCellsY, _TotalCellsZ;
		CellIndex _TotalCellsXY;
		CellIndex _TotalCellsXYZ;
	};


}


