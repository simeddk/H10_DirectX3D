#pragma once

class ModelRender
{
public:
	ModelRender(Shader* shader);
	~ModelRender();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);

	Transform* GetTrasnform() { return transform; }
	Model* GetModel() { return model; }

	void Pass(UINT pass);

private:
	void UpdateTransforms(); //Copy to All Bones to ModelMesh::boneBuffer(cbuffer)
	

private:
	bool bRead = false;
	
	Shader* shader;
	Model* model;

	Transform* transform; //Root Bone

	Matrix transforms[MAX_MODEL_BONES]; //All Local Bones
};