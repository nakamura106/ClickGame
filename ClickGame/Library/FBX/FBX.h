#ifndef FBX_H
#define FBX_H
#include <d3dx9.h>
#include <string>
#include <vector>
#include <fbxsdk.h>
#include <map>
#include <iostream>
#include"../Graphics/Graphics.h"

//頂点の情報
struct VERTEX_3D {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 nor;
	D3DCOLOR    col;
	float u, v;
	// スキンメッシュ用
	BYTE index[4];
	float weight[4];

};

// ボーン情報
struct Bone
{
	char		name[64];
	D3DXMATRIX	offset;
	D3DXMATRIX	transform;
};
//マテリアルの情報
struct MaterialData
{
	Graphics::TEXTURE_DATA texture_data;	// テクスチャ―
	D3DMATERIAL9 material;		// マテリアル
};

//メッシュの情報
struct MeshData {
	//ポリゴンの数
	int polygon_count;
	//頂点の数
	int vertex_count;
	//頂点インデックスの数
	int index_count;
	// 1頂点辺りのサイズ
	int vertex_stride;
	//マテリアル番号
	int material_index;

	int uv_set_count;
	//頂点
	VERTEX_3D* vertex;
	//頂点バッファ
	IDirect3DVertexBuffer9* vb_ptr;
	//インデックスバッファ
	IDirect3DIndexBuffer9* ib_ptr;
};
static const int BONE_MAX = 256;

//	アニメーション
struct Motion
{
	Motion()
	{
		ZeroMemory(key_ptr, sizeof(key_ptr));
	}

	UINT		num_frame;		// フレーム数	
	D3DXMATRIX* key_ptr[BONE_MAX];	// キーフレーム
};



//FBXの情報
struct FbxInfo {
	//メッシュ
	MeshData* mesh_ptr;
	//メッシュの数
	int mesh_count;
	//マテリアル
	MaterialData* material_ptr;
	//マテリアルの数
	int material_count;

	Bone bone[BONE_MAX];	// ボーン情報
	int bone_count;
	int	start_frame;		// 開始フレーム
	std::map<std::string, Motion>* motion_ptr;		// モーション
	D3DXMATRIX						world;			// ワールドマトリックス
};

struct FBXMeshData
{
	FbxInfo fbxinfo;
	char	motion[64];	// モーション名
	float	frame;		// フレーム
};



enum class Object
{
	PLAYER,
	MAX_OBJECT
};

class Fbx
{
public:
	/*Fbx(D3DXVECTOR3 Pos, D3DXVECTOR3 Scale):
		pos_(Pos),
		scale_(Scale)
	{}*/

	Fbx() {}

	~Fbx() {}



	//FBX準備
	FBXMeshData LoadFbx(const char* file_name_);

	bool LoadMesh(MeshData* pMeshData_, FbxMesh* pMesh_);

	void ReleaseFbxMesh(FBXMeshData* pData_);

	void GetIndeces(MeshData* pMeshData_, FbxMesh* pMesh_);
	//頂点情報取得
	void GetVertex(MeshData* pMeshData_, FbxMesh* pMesh_);
	//法線情報取得
	void GetNormal(MeshData* pMeshData_, FbxMesh* pMesh_);
	//UV座標の取得
	void GetUV(MeshData* pMeshData_, FbxMesh* pMesh_);
	//カラー取得
	void GetColor(MeshData* pMeshData_, FbxMesh* pMesh_);

	int FindBone(FbxInfo* pModel_, const char* pName_);

	void GetBone(FbxInfo* pModel_, MeshData* pMeshData_, FbxMesh* pMesh_);

	void GetKeyFrames(FbxInfo* pModel_, std::string name_, int bone_, FbxNode* pBoneNode_);

	void Play(FBXMeshData* pData_, std::string name_);
	//テクスチャ情報取得
	void GetTextureInfo(MaterialData* pMaterialData_, FbxMesh* pMesh_);
	//アニメーション無し
	void RenderFbxMesh(FBXMeshData* pData_);

	void DrawModel(FbxInfo* pModel_);

	void Skinning(FBXMeshData* pData_);

	void MatrixInterporate(D3DXMATRIX& out_, D3DXMATRIX& A_, D3DXMATRIX B_, float rate_);

	void ReleaseModel(FbxInfo* pModel_);

	void Animate(FBXMeshData* pData_, float sec_);

	void ResetAnimate(FBXMeshData* pData_);
private:




	D3DXVECTOR3 pos_;
	D3DXVECTOR3 scale_;

	char root_path_[MAX_PATH]; // ファイルのパス
};

#endif