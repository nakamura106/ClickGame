#ifndef Difinition_H_
#define Difinition_H_

#include"../Manager/LibraryManager.h"
#include"../Manager/SoundManager.h"
#include"../Manager/FbxManager.h"
#include"../Manager/SceneManager.h"

#define THE_FBX FBXManager::Instance()
#define THE_SCENE SceneManager::Instance()
#define THE_ENGINE LibraryManager::Instance()->GetEngine()
#define THE_INPUT LibraryManager::Instance()->GetInput()
#define THE_GRAPHICS LibraryManager::Instance()->GetGraphics()
#define THE_SOUND SoundManager::Instance()
#define THE_CAMERA LibraryManager::Instance()->GetCamera()
#define THE_VIDEO LibraryManager::Instance()->GetVideo()

#define ENEMY_GENERATE_RANGE_X 1800
#define ENEMY_GENERATE_RANGE_Y 1000

#define BG_POS_X 0
#define BG_POS_Y 0

#endif // !Difinition_H_
