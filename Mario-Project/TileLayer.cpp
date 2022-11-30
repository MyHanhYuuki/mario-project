#include "TileLayer.h"

void CTileLayer::Render()
{
	// Render all sprite on layer
	for (int i = 0; i < layer.size(); i++)
	{
		layer[i]->Render();
	}
}
