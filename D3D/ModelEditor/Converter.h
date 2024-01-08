#pragma once

class Converter
{
//-----------------------------------------------------
//Create Scene
//-----------------------------------------------------
public:
	Converter();
	~Converter();

	//Read Model File(*.fbx, *.obj, *.etc...)
	void ReadFile(wstring file);

//-----------------------------------------------------
//Read Mesh Data from scene -> Write to *.mesh file
//-----------------------------------------------------
public:
	void ExportMesh(wstring savePath);

private:
	void ReadBoneData(aiNode* node, int index, int parent); //aiNode is Mesh Bone
	void ReadMeshData(aiNode* node, int index);
	void WriteMeshData(wstring savePath);



private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
};