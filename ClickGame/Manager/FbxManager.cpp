#include"FbxManager.h"



FBXManager* FBXManager::p_instance = 0;

FBXManager* FBXManager::Instance()
{
	if (p_instance == 0)
	{
		p_instance = new FBXManager();
	}

	return p_instance;
}

FBXManager::FBXManager()
{
	if (fbx == nullptr)
	{
		fbx = new Fbx();
	}
		
}

FBXManager::~FBXManager()
{
	if (fbx != nullptr)
	{
		delete fbx;
		fbx = nullptr;
	}
}



FBXMeshData FBXManager::LoadFbxMesh(std::string key_, const char* pFilename_)
{
	if (HasKey(key_))
	{
		return MeshData[key_];
	}
	MeshData[key_] = fbx->LoadFbx(pFilename_);
	return MeshData[key_];
}

void FBXManager::ReleaseFbxMesh(std::string key_)
{
	fbx->ReleaseFbxMesh(&MeshData[key_]);
}

void FBXManager::AllReleaseMesh(FBXMeshData* meshdata_)
{
	if (meshdata_ == nullptr) { return; }
	fbx->ReleaseFbxMesh(meshdata_);
}

void FBXManager::DrawFbx(std::string key_, D3DXMATRIX& mat_world_)
{
	MeshData[key_].fbxinfo.world = mat_world_;
	fbx->RenderFbxMesh(&MeshData[key_]);
}

void FBXManager::Animation(std::string key_, float sec_)
{

	fbx->Animate(&MeshData[key_], sec_);

}

void FBXManager::ResetAnimation(FBXMeshData* meshdata_)
{
	if (meshdata_ == nullptr) { return; }
	fbx->ResetAnimate(meshdata_);
}

bool FBXManager::HasKey(std::string key_)
{
	auto itr = MeshData.find(key_);
	if (itr != MeshData.end())
	{
		return true;
	}
	return false;
}
