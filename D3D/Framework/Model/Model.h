#pragma once

#define MAX_MODEL_BONES 250
#define MAX_MODEL_FRAMES 500

class ModelBone;
class ModelMesh;
class ModelMeshPart;
class ModelClip;

class Model
{
public:
	friend class ModelRender;
	friend class ModelAnimator;

	struct VertexModel;

private:
	Model();
	~Model();

private:
	//Read *.mesh File
	void ReadMesh(wstring file);

	//Read *.material File
	void ReadMaterial(wstring file);

	//Read *.clip File
	void ReadClip(wstring file);

private:
	void BindBone(); //Make Tree
	void BindMesh(); //Call ModelMesh::Binding

public:
	UINT BoneCount() { return bones.size(); }
	vector<ModelBone*>& Bones() { return bones; }
	ModelBone* BoneByIndex(UINT index) { return bones[index]; }
	ModelBone* BoneByName(wstring name);

	UINT MeshCount() { return meshes.size(); }
	vector<ModelMesh*>& Meshes() { return meshes; }
	ModelMesh* MeshByIndex(UINT index) { return meshes[index]; }

	UINT MaterialCount() { return materials.size(); }
	vector<Material*>& Materials() { return materials; }
	Material* MaterialByIndex(UINT index) { return materials[index]; }
	Material* MaterialByName(wstring name);

	UINT ClipCount() { return clips.size(); }
	vector<ModelClip*>& Clips() { return clips; }
	ModelClip* ClipByIndex(UINT index) { return clips[index]; }
	ModelClip* ClipByName(wstring name);

public:
	struct VertexModel
	{
		Vector3 Position;
		Vector2 Uv;
		Vector3 Normal;
		Vector3 Tangent;
		Vector4 BlendIndices;
		Vector4 BlendWeights;

		VertexModel()
			: Position(0, 0, 0)
			, Uv(0, 0)
			, Normal(0, 0, 0)
			, Tangent(0, 0, 0)
			, BlendIndices(0, 0, 0, 0)
			, BlendWeights(0, 0, 0, 0)
		{

		}
	};

private:
	ModelBone* root;

	vector<ModelBone*> bones;
	vector<ModelMesh*> meshes;
	vector<Material*> materials;
	vector<ModelClip*> clips;
};