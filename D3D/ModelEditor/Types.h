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