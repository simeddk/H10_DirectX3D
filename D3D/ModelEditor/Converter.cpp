#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

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

	for (asBone* bone : bones)
		SafeDelete(bone);

	for (asMesh* mesh : meshes)
	{
		for (asMeshPart* part : mesh->MeshParts)
			SafeDelete(part);

		SafeDelete(mesh);
	}
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
	//Ignore Unused Bone
	if (node->mNumMeshes < 1) return;

	//Get Mesh Data
	asMesh* mesh = new asMesh();
	mesh->BoneIndex = index;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		UINT startVertex = mesh->Vertices.size();
		UINT startIndex = mesh->Indices.size();

		//Get Vertex Data
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			Model::VertexModel vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		//Get Index Data
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indices.push_back(face.mIndices[k] + startVertex);
		}
		
		//Get MeshPart Data from Material Name
		aiMaterial* materail = scene->mMaterials[srcMesh->mMaterialIndex];

		asMeshPart* meshPart = new asMeshPart();
		meshPart->MatrialName = materail->GetName().C_Str();
		meshPart->StartVertex = startVertex;
		meshPart->StartIndex = startIndex;
		meshPart->VertexCount = srcMesh->mNumVertices;
		meshPart->IndexCount = srcMesh->mNumFaces * srcMesh->mFaces->mNumIndices;

		mesh->MeshParts.push_back(meshPart);
	}

	meshes.push_back(mesh);
}

void Converter::WriteMeshData(wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));

	BinaryWriter* w = new BinaryWriter(savePath);

	//Save Bones
	w->UInt(bones.size());
	for (asBone* bone : bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);
		w->Int(bone->Parent);
		w->Matrix(bone->Transform);
	}

	//Save Meshes
	w->UInt(meshes.size());
	for (asMesh* mesh : meshes)
	{
		w->Int(mesh->BoneIndex);

		w->UInt(mesh->Vertices.size());
		w->Byte(&mesh->Vertices[0], sizeof(Model::VertexModel) * mesh->Vertices.size());

		w->UInt(mesh->Indices.size());
		w->Byte(&mesh->Indices[0], sizeof(UINT) * mesh->Indices.size());

		w->UInt(mesh->MeshParts.size());
		for (asMeshPart* part : mesh->MeshParts)
		{
			w->String(part->MatrialName);

			w->UInt(part->StartVertex);
			w->UInt(part->VertexCount);

			w->UInt(part->StartIndex);
			w->UInt(part->IndexCount);
		}
	}

	SafeDelete(w);
}

void Converter::ExportMaterial(wstring savePath, bool bOverWirte)
{
	savePath = L"../../_Textures/" + savePath + L".material";

	if (bOverWirte == false)
	{
		if (Path::ExistFile(savePath) == true)
			return;
	}

	ReadMaterialData();
	WriteMaterialData(savePath);
}

void Converter::ReadMaterialData()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		
		if (FoundMaterialData(srcMaterial) == false)
			continue;

		asMaterial* material = new asMaterial();
		material->Name = srcMaterial->GetName().C_Str();

		//Get ADSE Color
		aiColor3D color;

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material->Ambient = Color(color.r, color.g, color.b, 1.f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->Diffuse = Color(color.r, color.g, color.b, 1.f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material->Specular = Color(color.r, color.g, color.b, 1.f);

		srcMaterial->Get(AI_MATKEY_SHININESS, material->Specular.a);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		material->Emissive = Color(color.r, color.g, color.b, 1.f);

		//Get Texture(Map) File Path
		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->DiffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SpecularFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->NormalFile = file.C_Str();

		materials.push_back(material);
	}
}

bool Converter::FoundMaterialData(aiMaterial* material)
{
	string materialName = material->GetName().C_Str();

	for (asMesh* mesh : meshes)
	{
		for (asMeshPart* part : mesh->MeshParts)
		{
			if (part->MatrialName == materialName)
				return true;
		}
	}

	return false;
}

void Converter::WriteMaterialData(wstring savePath)
{
	string folder = String::ToString(Path::GetDirectoryName(savePath));
	string file = String::ToString(Path::GetFileName(savePath));

	Path::CreateFolders(folder);

	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	Xml::XMLElement* root = document->NewElement("Materials");
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for (asMaterial* material : materials)
	{
		//<Material>
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = nullptr;

		//<Name>
		element = document->NewElement("Name");
		element->SetText(material->Name.c_str());
		node->LinkEndChild(element);

		//<DiffuseFile>
		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, material->DiffuseFile).c_str());
		node->LinkEndChild(element);

		//<SpecularFile>
		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, material->SpecularFile).c_str());
		node->LinkEndChild(element);

		//<NormalFile>
		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, material->NormalFile).c_str());
		node->LinkEndChild(element);

		//<Ambient>
		element = document->NewElement("Ambient");
		element->SetAttribute("R", material->Ambient.r);
		element->SetAttribute("G", material->Ambient.g);
		element->SetAttribute("B", material->Ambient.b);
		element->SetAttribute("A", material->Ambient.a);
		node->LinkEndChild(element);

		//<Diffuse>
		element = document->NewElement("Diffuse");
		element->SetAttribute("R", material->Diffuse.r);
		element->SetAttribute("G", material->Diffuse.g);
		element->SetAttribute("B", material->Diffuse.b);
		element->SetAttribute("A", material->Diffuse.a);
		node->LinkEndChild(element);

		//<Specular>
		element = document->NewElement("Specular");
		element->SetAttribute("R", material->Specular.r);
		element->SetAttribute("G", material->Specular.g);
		element->SetAttribute("B", material->Specular.b);
		element->SetAttribute("A", material->Specular.a);
		node->LinkEndChild(element);

		//<Emissive>
		element = document->NewElement("Emissive");
		element->SetAttribute("R", material->Emissive.r);
		element->SetAttribute("G", material->Emissive.g);
		element->SetAttribute("B", material->Emissive.b);
		element->SetAttribute("A", material->Emissive.a);
		node->LinkEndChild(element);
	}

	document->SaveFile((folder + file).c_str());
	SafeDelete(document);
}

string Converter::WriteTexture(string saveFolder, string file)
{
	if (file.length() < 1) return "";

	string fileName = Path::GetFileName(file);
	const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(file.c_str());

	string path = "";

	if (embeddedTexture != nullptr)
	{
		path = saveFolder + Path::GetFileNameWithoutExtension(file) + ".png";

		BinaryWriter* w = new BinaryWriter(String::ToWString(path));
		w->Byte(embeddedTexture->pcData, embeddedTexture->mWidth);
		SafeDelete(w);
	}
	else
	{
		//"../../_Assets/Tower/"
		string directory = Path::GetDirectoryName(String::ToString(this->file)); 
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");

		if (Path::ExistFile(origin) == false)
			return "";

		path = saveFolder + fileName;
		CopyFileA(origin.c_str(), path.c_str(), FALSE);
		
		String::Replace(&path, "../../_Textures/", "");
	}

	return Path::GetFileName(path);
}
