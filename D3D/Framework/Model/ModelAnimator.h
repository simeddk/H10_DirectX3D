#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

public:
	void SetShader(Shader* shader, bool bFirst = false);
	void Pass(UINT pass);
	Transform* GetTransform() { return transform; }
	Model* GetModel() { return model; }

private:
	void CreateTexture();
	void CreateClipTransforms(UINT clipIndex);

private:
	struct ClipTranform
	{
		Matrix** Transform;

		ClipTranform()
		{
			Transform = new Matrix*[MAX_MODEL_FRAMES];

			for (UINT i = 0; i < MAX_MODEL_FRAMES; i++)
				Transform[i] = new Matrix[MAX_MODEL_BONES];
		}

		~ClipTranform()
		{
			for (UINT i = 0; i < MAX_MODEL_FRAMES; i++)
				SafeDeleteArray(Transform[i]);

			SafeDeleteArray(Transform);
		}
	};
	ClipTranform* clipTransforms;

private:
	ID3D11Texture2D* texture = nullptr;

private:
	Shader* shader;
	Model* model;

	Transform* transform; //Root Bone
};