#include "TileMap.h"
#include "Textures.h"

CTileMap::CTileMap()
{ 
	this->sprite_id_start = SPRITE_ID_START_TILEMAP; 
}

CTileMap::CTileMap(string path)
{ 
	this->filePath = path; 
	this->sprite_id_start = SPRITE_ID_START_TILEMAP; 
	
	// Extract and register sprite info from tile map
	MakeLayers();

	DebugOut(L"[info][TileMap] Load %d player(s) from %s \n", this->layers.size());
}

void CTileMap::MakeLayers()
{
	// Load xml file 
	TiXmlDocument doc(this->filePath.c_str());
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}

	// Lay thong tin node goc
	TiXmlElement* root = doc.RootElement();

	for (auto layer = root->FirstChildElement("layer"); layer != NULL; layer = layer->NextSiblingElement("layer"))
	{
		// Lấy tọa độ những chỗ cần vẽ trên map
		vector<vector<int>> grid = GetLayerInfo(layer);


		//Lấy tọa độ của các sprite cần vẽ
		GetSpriteInfo(root, grid);
	}
}


// Load tag data của layer để điền vào grid 2D [width, height]
vector<vector<int>> CTileMap::GetLayerInfo(TiXmlElement* layer_child)
{
	int numCellWidth, numCellHeight;
	layer_child->QueryIntAttribute("width", &numCellWidth);
	layer_child->QueryIntAttribute("height", &numCellHeight);

	string content = layer_child->FirstChildElement("data")->GetText();
	vector<string> num_tokens = Split(content, ",");

	// Vị trí thứ i của num_tokens sẽ là ô có sprite
	// Giá trị tại vị trí i:
	// Bằng 0: ô này ko có sprite
	// Khác 0: ô này có sprite, giá trị này là số thứ tự (bắt đầu từ 1, ko phải 0) của sprite trong ảnh texture

	// Điền vào grid theo trình tự: trái -> phải, trên -> dưới
	vector<vector<int>> grid(numCellWidth, vector<int>(numCellHeight));
	for (int i = 0; i < numCellWidth; i++)
	{
		for (int j = 0; j < numCellHeight; j++)
		{
			grid[i][j] = stoi(num_tokens[int(i + (j * numCellWidth))]);
		}
	}
	return grid;
}


void CTileMap::GetSpriteInfo(TiXmlElement* root, vector<vector<int>> grid)
{
	// Lấy tên file tileset để mở file đó và lấy các thông tin cần thiết
	TiXmlElement* tileset_layer = root->FirstChildElement("tileset");
	string img_tileset_path = tileset_layer->Attribute("source");

	TiXmlDocument doc_img(("scenes//" + img_tileset_path).c_str());
	if (!doc_img.LoadFile())
	{
		printf("%s", doc_img.ErrorDesc());
		return;
	}

	TiXmlElement* tile_set_ele = doc_img.RootElement();
	int spacing, margin, tileWidth, tileHeight, column;
	tile_set_ele->QueryIntAttribute("spacing", &spacing);
	tile_set_ele->QueryIntAttribute("margin", &margin);
	tile_set_ele->QueryIntAttribute("tilewidth", &tileWidth);
	tile_set_ele->QueryIntAttribute("tileheight", &tileHeight);
	tile_set_ele->QueryIntAttribute("columns", &column);


	CSprites* sprites = CSprites::GetInstance();
	LPTEXTURE texMap = CTextures::GetInstance()->Get(ID_FIRST_WORLD_MAP_1_1);

	int width = grid.size();
	int height = grid[0].size();
	LPTILELAYER tile_layer = new CTileLayer();

	// Duyệt từ trái -> phải, trên -> dưới để có thể dễ dàng debug (vì ta đã biết rõ thứ tự của các sprite trong layer)
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			int sprite_ith = grid[i][j];	// Vị trí của sprite (ith) trong ảnh texture bắt đầu từ số 1, ko phải số 0
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
				sprites->Add(this->sprite_id_start, left, top, right, bottom, texMap);

				// Thêm sprite vào layer
				LPTILE tile = new CTile(i, j, tileWidth, tileHeight, this->sprite_id_start);
				tile_layer->AddTile(tile);

				this->sprite_id_start++;
			}
		}
	}
	this->layers.push_back(tile_layer);
}

void CTileMap::RenderBackground()
{
	CSprites* sprites = CSprites::GetInstance();
	this->layers[0]->Render();
}

void CTileMap::RenderForgeground()
{
	CSprites* sprites = CSprites::GetInstance();
	this->layers[1]->Render();
}