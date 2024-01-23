#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();

private:
	void UpdateAnimationFrame();
	void UpdateBlendingFrame();

public:
	void ReadMesh(wstring file);
	void ReadMaterial(wstring file);
	void ReadClip(wstring file);

	void PlayTweenMode(UINT nextClip, float nextClipSpeed = 1.f, float takeTime = 0.1f);
	void PlayBlendMode(UINT clip1, UINT clip2, UINT clip3);
	void SetBlendAlpha(float alpha);

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
	ID3D11ShaderResourceView* transformsSRV;
	ID3DX11EffectShaderResourceVariable* sTransformsSRV;

private:
	//Single Clip
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.f;
		float RunningTime = 0.f;

		float Speed = 1.f;

		Vector2 Padding;
	};

	//Double Clip
	struct TweeDesc
	{
		float TakeTime = 0.1f;	// Requirement Time
		float TweenTime = 0.f;	// ChangeTime/TakeTime (Lerp C)
		float ChangeTime = 0.f; // Running Time
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweeDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}

	} tweeDesc;

	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

	//BlendSpace
private:
	struct BlendDesc
	{
		UINT Mode = 0;
		float Alpha = 0;
		Vector2 Padding;

		KeyframeDesc Clip[3];
	} blendDesc;

	ConstantBuffer* blendBuffer;
	ID3DX11EffectConstantBuffer* sBlendBuffer;

private:
	Shader* shader;
	Model* model;

	Transform* transform; //Root Bone
};