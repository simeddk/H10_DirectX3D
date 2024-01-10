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

//----------------------------------------------------------
//Read Mesh Data from scene -> Write to *.mesh(BIN) file
//----------------------------------------------------------
public:
	void ExportMesh(wstring savePath);

private:
	void ReadBoneData(aiNode* node, int index, int parent); //aiNode is Mesh Bone
	void ReadMeshData(aiNode* node, int index);
	void WriteMeshData(wstring savePath);

//----------------------------------------------------------
//Read Matrial Data from scene -> Write to *.material(XML) file
//----------------------------------------------------------
public:
	void ExportMaterial(wstring savePath, bool bOverWirte = true);

private:
	void ReadMaterialData();
	bool FoundMaterialData(aiMaterial* material); //Ignore Unused Matrial
	void WriteMaterialData(wstring savePath);
	string WriteTexture(string saveFolder, string file); //Copy to Texture Folder

private:
	wstring file;

	Assimp::Importer* importer;
	const aiScene* scene;

	vector<struct asBone*> bones;
	vector<struct asMesh*> meshes;
	vector<struct asMaterial*> materials;
};