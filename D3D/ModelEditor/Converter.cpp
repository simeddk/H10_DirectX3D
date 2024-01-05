#include "stdafx.h"
#include "Converter.h"
#include "Types.h"

//-----------------------------------------------------
//Create Scene
//-----------------------------------------------------
Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);
}

void Converter::ReadFile(wstring file)
{
	this->file = L"../../_Assets/" + file;

	scene = importer->ReadFile
	(
		String::ToString(this->file),
		aiProcess_ConvertToLeftHanded	|
		aiProcess_Triangulate			|
		aiProcess_GenUVCoords			| 
		aiProcess_GenNormals			|
		aiProcess_CalcTangentSpace
	);

	if (scene == nullptr)
	{
		string errorMsg = importer->GetErrorString();
		MessageBoxA(D3D::GetDesc().Handle, errorMsg.c_str(), "Model Import Error", MB_OK);

		exit(-1);
	}
}

//-----------------------------------------------------
//Read Mesh Data from scene -> Write to *.mesh file
//-----------------------------------------------------
void Converter::ExportMesh(wstring savePath)
{
	ReadBoneData(scene->mRootNode, -1, -1);
	WriteMeshData(L"../../_Models/" + savePath + L".mesh");

	//csv Test
	{
		FILE* buffer;
		fopen_s(&buffer, "../Bones.csv", "w");

		for (asBone* bone : bones)
		{
			fprintf(buffer, "%d,%d,%s\n", bone->Index, bone->Parent, bone->Name.c_str());
		}

		fclose(buffer);
	}
}

void Converter::ReadBoneData(aiNode* node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();
	
	Matrix transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent; //Component Space
	bones.push_back(bone);

	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);
}

void Converter::ReadMeshData(aiNode* node, int index)
{
	//UnresolvedMergeConflict. 24242424
}

void Converter::WriteMeshData(wstring savePath)
{
}
