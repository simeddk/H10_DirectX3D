#pragma once

#include "stdafx.h"

//-----------------------------------------------------------------------------
//Bone
//-----------------------------------------------------------------------------
struct asBone
{
	int Index;
	string Name;

	int Parent;
	Matrix Transform;
};

//-----------------------------------------------------------------------------
//Mesh
//-----------------------------------------------------------------------------
struct asMeshPart
{
	string MatrialName;

	UINT StartVertex;
	UINT VertexCount;
	UINT StartIndex;
	UINT IndexCount;
};

struct asMesh
{
	int BoneIndex;

	vector<Model::VertexModel> Vertices;
	vector<UINT > Indices;

	vector<asMeshPart*> MeshParts;
};

//-----------------------------------------------------------------------------
//Skinning
//-----------------------------------------------------------------------------
// -> Save to Vertex(Final)
struct asBlendWeight
{
	Vector4 Indices = Vector4(0, 0, 0, 0);
	Vector4 Weigths = Vector4(0, 0, 0, 0);

	void Set(UINT iterator, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (iterator)
		{
			case 0: Indices.x = i; Weigths.x = w;  break;
			case 1: Indices.y = i; Weigths.y = w; break;
			case 2: Indices.z = i; Weigths.z = w; break;
			case 3: Indices.w = i; Weigths.w = w; break;
		}
	}
};

// -> Calculate Skinning Data
struct asBoneWeight
{
private:
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	//Sort By Descending BoneWeights value
	void AddWeights(UINT boneIndex, float boneWeights)
	{
		if (boneWeights <= 0.f) return;

		bool bAdd = false;

		//At Not First
		vector<Pair>::iterator it = BoneWeights.begin();
		while (it != BoneWeights.end())
		{
			if (boneWeights > it->second)
			{
				BoneWeights.insert(it, Pair(boneIndex, boneWeights));
				bAdd = true;

				break;
			}

			it++;
		}
		
		//At First
		if (bAdd == false)
			BoneWeights.push_back(Pair(boneIndex, boneWeights));
	}

	void GetBlendWeigts(asBlendWeight& blendWeights)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeights.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	//UnresolvedMergeConflict. 가중치 정보를 0~1 사이 값으로 비율화를 하겠음..
	void Normalize()
	{

	}
};

//-----------------------------------------------------------------------------
//Material
//-----------------------------------------------------------------------------
struct asMaterial
{
	string Name;

	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	string DiffuseFile;
	string SpecularFile;
	string NormalFile;
};

//-----------------------------------------------------------------------------
//Clip
//-----------------------------------------------------------------------------
//-> 1 Bone, 1 Frame
struct asKeyFrameData
{
	float Frame;

	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

// -> 1 Bone, All Frame
struct asKeyFrame
{
	string BoneName;
	vector<asKeyFrameData> Transforms;
};

// -> All Bone, All Frame(Final)
struct asClip
{
	string Name;

	UINT FrameCount;
	float FrameRate;

	vector<asKeyFrame*> keyframes;
};

// -> 1 Bone, All Frame(Retarget)
struct asClipNode
{
	aiString Name;
	vector<asKeyFrameData> Keyframes;
};