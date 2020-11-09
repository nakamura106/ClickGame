#include"LibraryManager.h"
#include"../Library/Engine/Camera.h"
#include"../Library/Engine/Engine.h"
#include"../Library/Graphics/Graphics.h"
#include"../Library/Input/Input.h"
#include"../Library/Sound/Sound.h"
#include"../Library/Video/Video.h"

LibraryManager* LibraryManager::instance_ = 0;

LibraryManager* LibraryManager::Instance()
{
	if (instance_ == 0)
	{
		instance_ = new LibraryManager;
	}
	return instance_;
}

LibraryManager::LibraryManager()
{
	camera_ptr_ = new Camera(pos);
	engine_ptr_ = new Engine;
	graphics_ptr_ = new Graphics;
	input_ptr_ = new Input;
	video_ptr_ = new Video;
}

void LibraryManager::Init()
{
	camera_ptr_ = new Camera(pos);
	engine_ptr_ = new Engine;
	graphics_ptr_ = new Graphics;
	input_ptr_ = new Input;
	video_ptr_ = new Video;
}

