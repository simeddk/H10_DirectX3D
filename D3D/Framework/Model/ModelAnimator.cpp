#include "Framework.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(Shader* shader)
	: shader(shader)
{
	model = new Model();
	transform = new Transform(shader);

	frameBuffer = new ConstantBuffer(&tweeDesc, sizeof(TweeDesc));
	blendBuffer = new ConstantBuffer(&blendDesc, sizeof(BlendDesc));
}

ModelAnimator::~ModelAnimator()
{
	SafeDelete(model);
	SafeDelete(transform);

	SafeDeleteArray(clipTransforms);
	SafeRelease(texture);
	SafeRelease(transformsSRV);

	SafeDelete(frameBuffer);
	SafeDelete(blendBuffer);
}

void ModelAnimator::Update()
{
	if (texture == nullptr)
	{
		SetShader(shader, true);
		CreateTexture();
	}

	if (blendDesc.Mode == 0)
		UpdateAnimationFrame();
	else
		UpdateBlendingFrame();

	for (ModelMesh* mesh : model->Meshes())
		mesh->Update();
}

void ModelAnimator::Render()
{
	frameBuffer->Render();
	sFrameBuffer->SetConstantBuffer(frameBuffer->Buffer());

	blendBuffer->Render();
	sBlendBuffer->SetConstantBuffer(blendBuffer->Buffer());

	sTransformsSRV->SetResource(transformsSRV);

	for (ModelMesh* mesh : model->Meshes())
	{
		mesh->SetTransform(transform);
		mesh->Render();
	}
}

void ModelAnimator::UpdateAnimationFrame()
{
	TweeDesc& desc = tweeDesc;
	
	//CurrentClip
	ModelClip* clip = model->ClipByIndex(desc.Curr.Clip);

	desc.Curr.RunningTime += Time::Delta();

	float time = 1.f / clip->FrameRate() / desc.Curr.Speed;
	if (desc.Curr.Time >= 1.f)
	{
		desc.Curr.RunningTime = 0.f;
		desc.Curr.CurrFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
		desc.Curr.NextFrame = (desc.Curr.CurrFrame + 1) % clip->FrameCount();
	}

	desc.Curr.Time = desc.Curr.RunningTime / time;

	//NextClip
	if (desc.Next.Clip > -1)
	{
		ModelClip* nextClip = model->ClipByIndex(desc.Next.Clip);

		desc.ChangeTime += Time::Delta();
		desc.TweenTime = desc.ChangeTime / desc.TakeTime;

		//Tweening Completed
		if (desc.TweenTime >= 1.f)
		{
			desc.Curr = desc.Next;

			desc.Next.Clip = -1;
			desc.Next.CurrFrame = 0;
			desc.Next.NextFrame = 0;
			desc.Next.Time = 0.f;
			desc.Next.RunningTime = 0.f;

			desc.ChangeTime = 0.f;
			desc.TweenTime = 0.f;
		}

		//Tweening InProgress
		else
		{
			desc.Next.RunningTime += Time::Delta();

			float time = 1.f / clip->FrameRate() / desc.Next.Speed;
			if (desc.Next.Time >= 1.f)
			{
				desc.Next.RunningTime = 0.f;
				desc.Next.CurrFrame = (desc.Next.CurrFrame + 1) % clip->FrameCount();
				desc.Next.NextFrame = (desc.Next.CurrFrame + 1) % clip->FrameCount();
			}

			desc.Next.Time = desc.Next.RunningTime / time;
		}
	}

}

void ModelAnimator::UpdateBlendingFrame()
{
	BlendDesc& desc = blendDesc;

	//Update 3 Clips Frame
	for (UINT i = 0; i < 3; i++)
	{
		ModelClip* clip = model->ClipByIndex(desc.Clip[i].Clip);

		desc.Clip[i].RunningTime += Time::Delta();

		float time = 1.f / clip->FrameRate() / desc.Clip[i].Speed;
		if (desc.Clip[i].Time >= 1.f)
		{
			desc.Clip[i].RunningTime = 0.f;
			desc.Clip[i].CurrFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
			desc.Clip[i].NextFrame = (desc.Clip[i].CurrFrame + 1) % clip->FrameCount();
		}

		desc.Clip[i].Time = desc.Clip[i].RunningTime / time;
	}
}

void ModelAnimator::ReadMesh(wstring file)
{
	model->ReadMesh(file);
}

void ModelAnimator::ReadMaterial(wstring file)
{
	model->ReadMaterial(file);
}

void ModelAnimator::ReadClip(wstring file)
{
	model->ReadClip(file);
}

void ModelAnimator::PlayTweenMode(UINT nextClip, float nextClipSpeed, float takeTime)
{
	blendDesc.Mode = 0;

	tweeDesc.TakeTime = takeTime;
	tweeDesc.Next.Clip = nextClip;
	tweeDesc.Next.Speed = nextClipSpeed;
}

void ModelAnimator::PlayBlendMode(UINT clip1, UINT clip2, UINT clip3)
{
	blendDesc.Mode = 1;

	blendDesc.Clip[0].Clip = clip1;
	blendDesc.Clip[1].Clip = clip2;
	blendDesc.Clip[2].Clip = clip3;
}

void ModelAnimator::SetBlendAlpha(float alpha)
{
	alpha = Math::Clamp(alpha, 0.f, 2.f);
	blendDesc.Alpha = alpha;
}

void ModelAnimator::SetShader(Shader* shader, bool bFirst)
{
	this->shader = shader;

	if (bFirst == false)
	{
		SafeDelete(transform);
		transform = new Transform(shader);
	}

	sTransformsSRV = shader->AsSRV("TransformsMap");

	sFrameBuffer = shader->AsConstantBuffer("CB_AnimationFrame");
	sBlendBuffer = shader->AsConstantBuffer("CB_BlendingFrame");

	for (ModelMesh* mesh : model->Meshes())
		mesh->SetShader(shader);
}

void ModelAnimator::Pass(UINT pass)
{
	for (ModelMesh* mesh : model->Meshes())
		mesh->Pass(pass);
}

void ModelAnimator::CreateTexture()
{
	clipTransforms = new ClipTranform[model->ClipCount()];

	for (UINT i = 0; i < model->ClipCount(); i++)
		CreateClipTransforms(i);

	//Create Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_BONES * 4;
		desc.Height = MAX_MODEL_FRAMES;
		desc.ArraySize = model->ClipCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; //-> 16Byte
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		UINT pageSize = MAX_MODEL_BONES * MAX_MODEL_FRAMES * sizeof(Matrix);
		void* p = VirtualAlloc(nullptr, pageSize * model->ClipCount(), MEM_RESERVE, PAGE_READWRITE);

		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			UINT start = c * pageSize;

			for (UINT k = 0; k < MAX_MODEL_FRAMES; k++)
			{
				void* temp = (BYTE*)p + MAX_MODEL_BONES * k * sizeof(Matrix) + start;
				VirtualAlloc(temp, MAX_MODEL_BONES * sizeof(Matrix), MEM_COMMIT, PAGE_READWRITE);
				memcpy(temp, clipTransforms[c].Transform[k], MAX_MODEL_BONES * sizeof(Matrix));
			}
		}

		D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[model->ClipCount()];
		for (UINT c = 0; c < model->ClipCount(); c++)
		{
			void* temp = (BYTE*)p + c * pageSize;
			subResource[c].pSysMem = temp;
			subResource[c].SysMemPitch = MAX_MODEL_BONES * sizeof(Matrix);
			subResource[c].SysMemSlicePitch = pageSize;
		}
		Check(D3D::GetDevice()->CreateTexture2D(&desc, subResource, &texture));

		SafeDelete(subResource);
		VirtualFree(p, 0, MEM_RELEASE);
	}

	//Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = model->ClipCount();

		Check(D3D::GetDevice()->CreateShaderResourceView(texture, &desc, &transformsSRV));
	}
}

void ModelAnimator::CreateClipTransforms(UINT clipIndex)
{
	Matrix* bones = new Matrix[MAX_MODEL_BONES];

	ModelClip* clip = model->ClipByIndex(clipIndex);

	for (UINT f = 0; f < clip->FrameCount(); f++)
	{
		for (UINT b = 0; b < model->BoneCount(); b++)
		{
			ModelBone* bone = model->BoneByIndex(b);

			Matrix inv = bone->Transform();
			D3DXMatrixInverse(&inv, nullptr, &inv);

			Matrix parent;
			int parentIndex = bone->ParentIndex();
			if (parentIndex < 0)
				D3DXMatrixIdentity(&parent);
			else
				parent = bones[parentIndex];

			Matrix animation;

			ModelKeyFrame* frame = clip->Keyframe(bone->Name());
			if (frame != nullptr)
			{
				ModelKeyFrameData& data = frame->Transforms[f];
				
				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);

				animation = S * R * T;
			}
			else
			{
				D3DXMatrixIdentity(&animation);
			}

			bones[b] = animation * parent;
			clipTransforms[clipIndex].Transform[f][b] = inv * bones[b];
			//inv * animation * parent;
			
		}
	}

	
}
