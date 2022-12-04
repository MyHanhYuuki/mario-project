#pragma once
#include <vector>
#include "Tile.h"
#include "tinyxml.h"

using namespace std;

class CTileLayer
{
private:
	vector<LPTILE> layer;
	int width;	//cell unit
	int height;	// cell unit
public:
	CTileLayer(int width, int height) { this->width = width; this->height = height; };
	void AddTile(LPTILE tile) { this->layer.push_back(tile); }
	void Render();

	vector<vector<int>> tileIDs;
};
typedef CTileLayer* LPTILELAYER;
