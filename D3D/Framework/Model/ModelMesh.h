#pragma once

//-----------------------------------------------------------------------------
// @ ModelBone
// Read *.mesh File -> Make Tree Like Structure
//-----------------------------------------------------------------------------
class ModelBone
{
public:
	friend class Model;

private:
	ModelBone() = default;
	~ModelBone() = default;

public:
	int Index() { return index; }
	wstring Name() { return name; }

	int ParentIndex() { return parentIndex; }
	ModelBone* Parent() { return parent; }

	Matrix& Transform() { return transform; }
	void Transform(Matrix& matrix) { transform = matrix; }

	vector<ModelBone*>& Children() { return children; }

private:
	int index;
	wstring name;

	int parentIndex;
	ModelBone* parent;

	Matrix transform;
	vector<ModelBone*> children;
};

//-----------------------------------------------------------------------------
// @ ModelMesh
// 1. Bone Index -> CBuffer Ready
// 2. Bining VertexBuffer, IndexBuffer
// 3. Render to MeshPart
//-----------------------------------------------------------------------------
class ModelMesh
{
public:
	friend class Model;

private:
	ModelMesh();
	~ModelMesh();

	//Bining VertexBuffer, IndexBuffer, Material....
	void Binding(Model* model);

public:
	void Pass(UINT val);

	void SetShader(Shader* shader);

	void Update();
	void Render();

	int BoneIndex() { return boneIndex; }
	ModelBone* Bone() { return bone; }

	//BoneSpace - Set All Bone Matrices to BoneBuffe(CBuffer)
	void Transforms(Matrix* transforms);
	
	//WorldSpace - Set Root Bone Transform
	void SetTransform(Transform* tranform);

private:
	struct BoneDesc
	{
		Matrix Trasnforms[MAX_MODEL_BONES]; //All Bone Matrices

		UINT BoneIndex; //This Mesh Sholud Be Attached Bone Index
		float Padding[3];
	} boneDesc;

private:
	Shader* shader;

	Transform* transform = nullptr; //Root Transform(Actor Trasnform)
	PerFrame* perFrame = nullptr;

	int boneIndex;
	ModelBone* bone;

	VertexBuffer* vertexBuffer;
	UINT vertexCount;
	Model::VertexModel* vertices;

	IndexBuffer* indexBuffer;
	UINT indexCount;
	UINT* indices;

	ConstantBuffer* boneBuffer;
	ID3DX11EffectConstantBuffer* sBoneBuffer;

	vector<class ModelMeshPart*> meshParts;
};

//-----------------------------------------------------------------------------
// @ ModelMeshPart
// 1. Draw Primitive Call
// 2. Bind Material
//-----------------------------------------------------------------------------
class ModelMeshPart
{
public:
	friend class Model;
	friend class ModelMesh;

private:
	ModelMeshPart();
	~ModelMeshPart();

	void Update();
	void Render();

	void Binding(Model* model);
	void SetShader(Shader* shader);

	void Pass(UINT val) { pass = val; }

private:
	Shader* shader;
	UINT pass = 0;

	Material* material = nullptr;
	wstring materialName;

	UINT startVertex;
	UINT vertexCount;

	UINT startIndex;
	UINT indexCount;
};