#ifndef FBXMANAGER_H_
#define FBXMANAGER_H_
#include"../Library/FBX/FBX.h"
#include<map>
#include<string>

class FBXManager
{
public:
	static FBXManager* Instance();

	FBXMeshData LoadFbxMesh(std::string key_, const char* pFilename_);

	void ReleaseFbxMesh(std::string key_);

	void AllReleaseMesh(FBXMeshData* meshdata_);

	void DrawFbx(std::string key_, D3DXMATRIX& mat_world_);

	void Animation(std::string key_, float sec_);

	void ResetAnimation(FBXMeshData* meshdata_);

	FBXMeshData GetMeshData(std::string key_) { return MeshData[key_]; }
private:
	FBXManager();

	~FBXManager();

	bool HasKey(std::string key_);

private:
	static FBXManager* p_instance;

	Fbx* fbx;

	std::map<std::string, FBXMeshData> MeshData;

	
};

#endif // !FBXMANAGER_H_
