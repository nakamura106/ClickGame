#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

class BaseScene;
class TitleScene;
class HelpScene;
class GameMainScene;
class GameEndScene;

enum class SceneId
{
	Title,
	Help,
	Game,
	End,
};

enum class SceneStep
{
	Init,
	Main,
	End,
};

class SceneManager
{
private:
	static SceneManager* p_instance;

	struct SceneInfo
	{
		SceneId CurrentSceneID = SceneId::Title;
		SceneStep CurrentSceneStep = SceneStep::Init;
	}Scene_Info;

	struct SceneInstance
	{
		TitleScene* title;
		HelpScene* help;
		GameMainScene* main;
		GameEndScene* end;
	}Scene_Instance;

public:
	static SceneManager* Instance();

	void Init();

	void Update();
	void Draw();

	const TitleScene* GetTitle() { return Scene_Instance.title; };
	const HelpScene* GetHelp() { return Scene_Instance.help; };
	const GameMainScene* GetGamaMain() { return Scene_Instance.main; };
	const GameEndScene* GetEnd() { return Scene_Instance.end; };

	void SetSceneStep(SceneStep step_) { Scene_Info.CurrentSceneStep = step_; };
	void SetSceneId(SceneId id_) { Scene_Info.CurrentSceneID = id_; };

protected:
	SceneManager();
	~SceneManager();

private:
	void InitScene(SceneId id_);
	void MainScene(SceneId id_);
	void EndScene(SceneId id_);
	void DrawScene(BaseScene* scene_);
};


#endif