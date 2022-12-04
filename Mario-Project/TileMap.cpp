#include "TileMap.h"
#include "Textures.h"
#include "InvisiblePlatform.h"
#include "AssetIDs.h"

CMap::CMap(string path)
{ 
	this->filePath = path; 
	this->firstSpriteId = SPRITE_ID_START_TILEMAP;
	this->background = NULL;
	this->forgeground = NULL;
	
	// load map
	Load();
}

void CMap::Load()
{
	TiXmlDocument doc(this->filePath.c_str());
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}

	TiXmlElement* root = doc.RootElement();

	// Load tileset
	TileSet* tileSet = this->LoadTileSet(root);
	if (tileSet == NULL) {
		DebugOut(L"[ERROR] fail to load tile set");
		return;
	}

	// Load layer
	for (auto element = root->FirstChildElement("layer"); element != NULL; element = element->NextSiblingElement("layer"))
	{
		// Load map info
		CTileLayer* layer = LoadLayer(element);
		if (layer != NULL) {
			string name = element->Attribute("name");

			// Hiện tile map chỉ có 2 layer
			if (name == "Background") {
				this->background = layer;
			}
			else if (name == "Foreground") {
				this->forgeground = layer;
			}
			else {
				this->layers.push_back(layer);
			}

			//Lấy tọa độ của các sprite cần vẽ
			registerTilesAndSprites(tileSet, layer);
		}
	}

	// Load object group
	for (auto element = root->FirstChildElement("objectgroup"); element != NULL; element = element->NextSiblingElement("objectgroup")) {
		string name = element->Attribute("name");
		int objectGroupdID;
		element->QueryIntAttribute("id", &objectGroupdID);

		if (name != "Solid") continue;
		CGameObject* gameObject = NULL;
		for (TiXmlElement* object = element->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object"))
		{
			int id, x, y, width, height;
			int type = 0;
			int cellX = -1, cellY = -1;
			object->QueryIntAttribute("id", &id);
			object->QueryIntAttribute("x", &x);
			object->QueryIntAttribute("y", &y);
			object->QueryIntAttribute("width", &width);
			object->QueryIntAttribute("height", &height);

			gameObject = new CInvisiblePlatform(x, y, OBJECT_TYPE_INVISIBLE_PLATFORM, width, height);
			this->objects.push_back(gameObject);
		}
	}
}

TileSet* CMap::LoadTileSet(TiXmlElement* root)
{
	// Lấy tên file tileset để mở file đó và lấy các thông tin cần thiết
	TiXmlElement* element = root->FirstChildElement("tileset");
	string imageSourcePath = element->Attribute("source");

	TiXmlDocument imageRootNode(("scenes//" + imageSourcePath).c_str());
	if (imageRootNode.LoadFile())
	{
		TiXmlElement* tileSetElement = imageRootNode.RootElement();

		TileSet* tileSet = new TileSet();
		tileSetElement->QueryIntAttribute("spacing", &tileSet->spacing);
		tileSetElement->QueryIntAttribute("margin", &tileSet->margin);
		tileSetElement->QueryIntAttribute("tilewidth", &tileSet->width);
		tileSetElement->QueryIntAttribute("tileheight", &tileSet->height);
		tileSetElement->QueryIntAttribute("columns", &tileSet->columns);

		return tileSet;
	}

	return NULL;
}

// Load tag data của layer để điền vào grid 2D [width, height]
CTileLayer* CMap::LoadLayer(TiXmlElement* layerElement)
{
	int visible;
	layerElement->QueryIntAttribute("visible", &visible);
	if (visible == 0) {
		return NULL;
	}

	int colCount, rowCount;
	layerElement->QueryIntAttribute("width", &colCount);
	layerElement->QueryIntAttribute("height", &rowCount);
	CTileLayer* layer = new CTileLayer(colCount, rowCount);

	string strTile = layerElement->FirstChildElement("data")->GetText();
	vector<string> tiles = split(strTile, ",");

	// Vị trí thứ i của num_tokens sẽ là ô có sprite
	// Giá trị tại vị trí i:
	// Bằng 0: ô này ko có sprite
	// Khác 0: ô này có sprite, giá trị này là số thứ tự (bắt đầu từ 1, ko phải 0) của sprite trong ảnh texture

	// Điền vào grid theo trình tự: trái -> phải, trên -> dưới
	vector<vector<int>> tileIDs(colCount, vector<int>(rowCount));
	for (int col = 0; col < colCount; col++)
	{
		for (int row = 0; row < rowCount; row++)
		{
			tileIDs[col][row] = stoi(tiles[int(col + (row * colCount))]);
		}
	}
	layer->tileIDs = tileIDs;

	// clear data
	tiles.clear();

	return layer;
}

void CMap::registerTilesAndSprites(TileSet* tileSet, CTileLayer* layer)
{
	int spacing = tileSet->spacing
		, margin = tileSet->margin
		, tileWidth = tileSet->width
		, tileHeight = tileSet->height
		, column = tileSet->columns;

	CSprites* sprites = CSprites::GetInstance();
	LPTEXTURE texMap = CTextures::GetInstance()->Get(ID_FIRST_WORLD_MAP_1_1);

	auto posTable = layer->tileIDs;
	int width = posTable.size();
	int height = posTable[0].size();

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int sprite_ith = posTable[i][j];	// Vị trí của sprite (ith) trong ảnh texture bắt đầu từ số 1, ko phải số 0
			if (sprite_ith != 0)	// Khác 0 tức là vị trí này có sprite
			{
				int x_th, y_th; // Bắt đầu từ số 0
				if (sprite_ith % column > 0) // chia dư, tức là sprite này nằm không nằm ở cột cuối cùng của tileset
				{
					x_th = sprite_ith % column - 1;	// vị trí của sprite trong tileset (sprite_ith) bắt đầu từ 1 nên cần trừ 1 để chuyển về bắt đầu từ 0
					y_th = sprite_ith / column;
				}
				else // nếu chia ko dư thì vị trí x_th của sprite nằm ở cột cuối cùng
				{
					x_th = column;
					y_th = sprite_ith / column - 1;
				}

				int left = margin + x_th * (tileWidth + spacing);
				int top = margin + y_th * (tileHeight + spacing);
				int right = left + tileWidth - 1;
				int bottom = top + tileHeight - 1;

				// Thêm sprite vào database
				sprites->Add(this->firstSpriteId, left, top, right, bottom, texMap);

				// Thêm sprite vào layer
				LPTILE tile = new CTile(i, j, tileWidth, tileHeight, this->firstSpriteId);
				layer->AddTile(tile);

				this->firstSpriteId++;
			}
		}
	}
}


void CMap::Render(vector<LPGAMEOBJECT> objects) {

	if (this->background != NULL)
		this->background->Render();

	for (int i = 0; i < layers.size(); i++) {
		layers[i]->Render();
	}

	// Vẽ theo chiều ngược lại vì Mario thuộc vị trí đầu tiên của objects, theo sau là các object và cuối cùng là map
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	for (int i = 0; i < this->objects.size(); i++)
		this->objects[i]->Render();

	if (this->forgeground != NULL)
		this->forgeground->Render();
}