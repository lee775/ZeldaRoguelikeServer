#include "pch.h"
#include "TileMap.h"
#include <fstream>
#include <iostream>

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

void TileMap::LoadFile(const wstring& path)
{
	//// C 스타일
	//{
	//	FILE* file = nullptr;
	//	::_wfopen_s(&file, path.c_str(), L"rb");
	//	assert(file != nullptr);

	//	// _mapSize.x
	//	::fread(&_mapSize.x, sizeof(_mapSize.x), 1, file);
	//	::fread(&_mapSize.y, sizeof(_mapSize.y), 1, file);

	//	SetMapSize(_mapSize);

	//	for (int32 y = 0; y < _mapSize.y; y++)
	//	{
	//		for (int32 x = 0; x < _mapSize.x; x++)
	//		{
	//			int32 value = -1;
	//			::fread(&value, sizeof(value), 1, file);
	//			_tiles[y][x].value = value;
	//		}
	//	}

	//	::fclose(file);
	//}

		// C++ 스타일
	{
		//Input File Stream
		wifstream ifs;

		ifs.open(path);

		ifs >> _mapSize.x >> _mapSize.y;

		SetMapSize(_mapSize);

		for (int32 y = 0; y < _mapSize.y; y++)
		{
			wstring line;
			ifs >> line;
			for (int32 x = 0; x < _mapSize.x; x++)
			{
				_tiles[y][x].value = line[x] - L'0';
			}
		}
		ifs.close();
	}
}

Tile* TileMap::GetTileAt(VectorInt pos)
{
	if (pos.x < 0 || pos.x >= _mapSize.x || pos.y < 0 || pos.y >= _mapSize.y)
		return nullptr;

	return &_tiles[pos.y][pos.x];
}

void TileMap::SetMapSize(VectorInt size)
{
	_mapSize = size;

	_tiles = vector<vector<Tile>>(size.y, vector<Tile>(size.x));

	for (int32 y = 0; y < size.y; y++)
	{
		for (int32 x = 0; x < size.x; x++)
		{
			_tiles[y][x] = Tile{ 0 };
		}
	}

	// 맵 사이즈가 기존이랑 다르게 바뀌면 추가작업 넣어줘야함
}

void TileMap::SetTileSize(int32 size)
{
	_tileSize = size;

	// 타일 사이즈가 바뀌면 추가 작업을 해줘야함
}
