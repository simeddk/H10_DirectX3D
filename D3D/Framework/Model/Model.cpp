#include "Framework.h"
#include "Model.h"
#include "Utilities/BinaryFile.h"


Model::Model()
{
}

Model::~Model()
{
	for (ModelBone* bone : bones)
		SafeDelete(bone);

	for (ModelMesh* mesh : meshes)
		SafeDelete(mesh);
}

void Model::ReadMesh(wstring file)
{
	file = L"../../_Models/" + file + L".mesh";

	BinaryReader* r = new BinaryReader(file);

	UINT count = 0;

	//Read Bone
	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->index = r->Int();
		bone->name = String::ToWString(r->String());
		bone->parentIndex = r->Int();
		bone->transform = r->Matrix();

		bones.push_back(bone);
	}

	//Read Mesh
	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();

		mesh->boneIndex = r->Int();

		//Vertex Data
		{
			UINT vertexCount = r->UInt();

			vector<Model::VertexModel> vertices;
			vertices.assign(vertexCount, Model::VertexModel());

			void* ptr = (void*)&vertices[0];
			r->Byte(&ptr, sizeof(Model::VertexModel) * vertexCount);

			mesh->vertices = new Model::VertexModel[vertexCount];
			mesh->vertexCount = vertexCount;

			copy
			(
				vertices.begin(),
				vertices.end(),
				stdext::checked_array_iterator<Model::VertexModel*>(mesh->vertices, vertexCount)
			);
		}

		//Index Data
		{
			UINT indexCount = r->UInt();

			vector<UINT> indices;
			indices.assign(indexCount, UINT());

			void* ptr = (void*)&indices[0];
			r->Byte(&ptr, sizeof(UINT) * indexCount);

			mesh->indices = new UINT[indexCount];
			mesh->indexCount = indexCount;

			copy
			(
				indices.begin(),
				indices.end(),
				stdext::checked_array_iterator<UINT*>(mesh->indices, indexCount)
			);
		}

		//MeshPart Data
		{
			UINT partCount = r->UInt();
			for (UINT p = 0; p < partCount; p++)
			{
				ModelMeshPart* part = new ModelMeshPart();
				part->materialName = String::ToWString(r->String());

				part->startVertex = r->UInt();
				part->vertexCount = r->UInt();

				part->startIndex = r->UInt();
				part->indexCount = r->UInt();

				mesh->meshParts.push_back(part);
			}
		}

		meshes.push_back(mesh);
	}

	SafeDelete(r);

	BindBone();
	BindMesh();
}

void Model::BindBone()
{
	root = bones[0];

	for (ModelBone* bone : bones)
	{
		//Exist Parent Bone
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->children.push_back(bone);
		}

		//Root Bone
		else
		{
			bone->parent = nullptr;
		}
	}
}

void Model::BindMesh()
{
	for (ModelMesh* mesh : meshes)
	{
		mesh->bone = bones[mesh->boneIndex];
		mesh->Binding(this);
	}
}

ModelBone* Model::BoneByName(wstring name)
{
	for (ModelBone* bone : bones)
	{
		if (name == bone->name)
			return bone;
	}

	return nullptr;
}
