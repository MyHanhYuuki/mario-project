#pragma once
#include <vector>
#include "Tile.h"
#include "tinyxml.h"

using namespace std;

class CTileLayer
{
private:
	vector<LPTILE> layer;
public:
	CTileLayer() {};
	void AddTile(LPTILE tile) { this->layer.push_back(tile); }
	void Render();
};
typedef CTileLayer* LPTILELAYER;
