#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "AssetIDs.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "InvisiblePlatform.h"
#include "SampleKeyEventHandler.h"
#include "tinyxml.h"
#include "QuestionBrick.h"
#include "Point.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	this->player = NULL;
	this->key_handler = new CSampleKeyHandler(this);
	this->base_platform_pos_y = 0;
	this->old_base_platform_pos_y = 0;
	this->worldWidth = 0;
}

#define SCENE_SECTION_UNKNOWN  -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_TILEMAP	2
#define SCENE_SECTION_OBJECTS	3

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

#define ASSETS_FOLDER "assets\\"
void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(ASSETS_FOLDER) + ToWSTR(tokens[0]);

	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() == 0) return;

	string path = tokens[0];

	LoadMap(path);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + (long long int)1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 2) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
	{
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object has been created!\n");

		MakeCameraFollowMario();
		break;
	}
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x, y); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(x, y); break;
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;
	case OBJECT_TYPE_PLATFORM:
	{
		bool is_first_base = false;

		// Nếu đây là platform base 
		// Camera không dịch chuyển theo trục y khi Mario đứng trên platform
		bool is_base = tokens.size() == 10 ? true : false;

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y, is_base,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);

		if (tokens.size() == 11)
		{
			int base_type = atoi(tokens[10].c_str());
			if (base_type == FIRST_BASE_PLATFORM)
				this->SetBasePlatformPosY(y);
			else if (base_type == HIDEEN_ZONE_BASE_PLATFORM)
			{
				itemsInside.insert(make_pair(DEFAULT_ID_HIDDEN_ZONE_BASE_PLATFORM, obj));
				DebugOut(L"Loaded HZ base platform\n");
			}
		}
		break;
	}
	case OBJECT_TYPE_INVISIBLE_PLATFORM:
	{
		int width = atoi(tokens[3].c_str());
		int height = atoi(tokens[4].c_str());
		obj = new CInvisiblePlatform(x, y, object_type, width, height);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);

		break;
	}
	break;

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);


	objects.push_back(obj);
}

void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines
		if (line.empty()) continue;	// skip empty lines
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[TILEMAP]") { section = SCENE_SECTION_TILEMAP; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_TILEMAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::LoadMap(string filePath)
{
	TiXmlDocument doc(filePath.c_str());
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
		return;
	}

	// Parse tile object
	auto root = doc.RootElement();
	ParseTile(doc.RootElement());

	// Load map property
	root->QueryIntAttribute("width", &worldWidth);

	// Load object group
	for (auto element = root->FirstChildElement("objectgroup"); element != NULL; element = element->NextSiblingElement("objectgroup")) {
		for (TiXmlElement* object = element->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement("object"))
		{
			CGameObject* gameObject = NULL;

			float x, y;
			int id, width, height;
			int type = 0;
			int cellX = -1, cellY = -1;
			string name;
			name = object->Attribute("name");
			object->QueryIntAttribute("id", &id);
			object->QueryFloatAttribute("x", &x);
			object->QueryFloatAttribute("y", &y);
			object->QueryIntAttribute("width", &width);
			object->QueryIntAttribute("height", &height);

			// Skip invisible object
			if (object->Attribute("visible")) {
				continue;
			}

			// Parse Platform and it's derived class
			if ((int)name.rfind("Platform") >= 0 || name == "pipe") {
				gameObject = new CInvisiblePlatform(x, y, OBJECT_TYPE_INVISIBLE_PLATFORM, width, height);

				if ((int)name.rfind("Top") >= 0) {
					gameObject->SetBlockDirection(BLOCK_TOP);
				}
			} else if (name == "Goomba") {
				gameObject = new CGoomba(x, y);
			} else if (name == "Coin") {
				gameObject = new CCoin(x, y);
			} else if ((int)name.rfind("QuestionBrick") >= 0) {
				gameObject = new CQuestionBrick(x, y);
			}
			
			if (gameObject) {
				gameObject->SetName(name);
				this->objects.push_back(gameObject);
			}
		}
	}
}

TileSet* CPlayScene::LoadTileSet(TiXmlElement* root)
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

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way
	/* "dirty" là vì chưa xử dụng phân hoạch không gian(Grid, Quad Tree, ...) để xử lý va chạm cho ít game object hơn.
		 Xử lý ít hơn là để cho game chạy nhanh hơn, mà cũng là vì những object không ở gần cửa sổ game thì cũng không cần update làm chi
		 (đỡ tốn thời gian tính toán cho CPU).
		 Nên mới cần phân hoạch không gian để chọn ra những object thực sự cần thiết để update.
	*/

	vector<LPGAMEOBJECT> coObjects;

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	MakeCameraFollowMario();

	PurgeDeletedObjects();
}

void CPlayScene::MakeCameraFollowMario()
{
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;

	// Get current mario
	CGame* game = CGame::GetInstance();
	game->GetCamPos(cx, cy);

	// Adjust camera base on mario position
	// whenever mario is alive
	if (player->GetState() != MARIO_STATE_DIE) {
		player->GetPosition(cx, cy);
		cx -= game->GetBackBufferWidth() / 2;
		cy -= game->GetBackBufferHeight() / 2;
	}

	if (cx < 0) {
		cx = 0;
	}
	else if ((cx  + game->GetBackBufferWidth() / 2) > worldWidth) {
		cx = worldWidth - game->GetBackBufferWidth() / 2.0f;
	}

	CGame::GetInstance()->SetCamPos(cx, /*0.0f*/ cy);
}

void CPlayScene::Render()
{
	for (int i = 0; i < tiles.size(); i++) {
		tiles[i]->Render();
	}

	// Vẽ theo chiều ngược lại vì Mario thuộc vị trí đầu tiên của objects, theo sau là các object và cuối cùng là map
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	for (int i = 0; i < this->objects.size(); i++)
	{
		if (objects[i]->IsDeleted() != true) {
			this->objects[i]->Render();
		}
	}
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	vector<LPGAMEOBJECT>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		delete (*it);
	}
	objects.clear();
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well

*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	// Các item trong itemsInside đã được xóa cùng với các game object khác khi xóa các game object trong objects
	// Nên bây giờ chỉ cần clear itemsInside mà thôi
	itemsInside.clear();

	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o->IsDeleted(); }

void CPlayScene::PurgeDeletedObjects()
{
	// NOTE: remove_if will swap all deleted items to the end of the vector
	// then simply trim the vector, this is much more efficient than deleting individual items
	objects.erase(
		std::remove_if(objects.begin(), objects.end(), CPlayScene::IsGameObjectDeleted),
		objects.end());
}

// Load tag data của layer để điền vào grid 2D [width, height]
void CPlayScene::ParseTile(TiXmlElement* root)
{
	TiXmlElement* layerElement = root->FirstChildElement("layer");

	int visible;
	layerElement->QueryIntAttribute("visible", &visible);
	if (visible == 0) {
		return;
	}

	int colCount, rowCount;
	layerElement->QueryIntAttribute("width", &colCount);
	layerElement->QueryIntAttribute("height", &rowCount);

	string strTile = layerElement->FirstChildElement("data")->GetText();
	vector<string> tileArr = split(strTile, ",");

	// Điền vào grid theo trình tự: trái -> phải, trên -> dưới
	vector<vector<int>> tileIDs(colCount, vector<int>(rowCount));
	for (int col = 0; col < colCount; col++)
	{
		for (int row = 0; row < rowCount; row++)
		{
			tileIDs[col][row] = stoi(tileArr[int(col + (row * colCount))]);
		}
	}
	// clear data
	tileArr.clear();

	// Load tileset
	TileSet* tileSet = LoadTileSet(root);
	if (tileSet == NULL) {
		DebugOut(L"[ERROR] fail to load tile set");
		return;
	}

	// Add tile object
	int spacing = tileSet->spacing
	, margin = tileSet->margin
	, tileWidth = tileSet->width
	, tileHeight = tileSet->height
	, column = tileSet->columns;

	CSprites* sprites = CSprites::GetInstance();
	LPTEXTURE texMap = CTextures::GetInstance()->Get(ID_TEX_IMAGEMAP);

	int firstSpriteId = SPRITE_ID_START_TILEMAP;
	for (int j = 0; j < tileIDs[0].size(); j++)
	{
		for (int i = 0; i < tileIDs.size(); i++)
		{
			// Vị trí của sprite (ith) trong ảnh texture bắt đầu từ số 1, ko phải số 0
			int sprite_ith = tileIDs[i][j];

			// Khác 0 tức là vị trí này có sprite
			if (sprite_ith != 0)
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
				sprites->Add(firstSpriteId, left, top, right, bottom, texMap);

				LPTILE tile = new CTile((float)i, (float)j, tileWidth, tileHeight, firstSpriteId++);
				tiles.push_back(tile);
			}
		}
	}
}

bool CPlayScene::AddGameObjectBefore(LPGAMEOBJECT baseObj, LPGAMEOBJECT newGameObj) {
	// Add new item into object list before this position (due to update order)
	auto index = find(objects.begin(), objects.end(), baseObj);
	if (index != objects.end()) {
		objects.insert(index, newGameObj);

		return true;
	}

	return false;
}

void CPlayScene::FireGainPointEvent(string eventName, float x, float y, int point)
{
	auto pointObj = new CPoint(x, y);
	pointObj->SetPoint(point);
	pointObj->SetState(POINT_STATE_BOUNCING);

	objects.push_back(pointObj);
}
