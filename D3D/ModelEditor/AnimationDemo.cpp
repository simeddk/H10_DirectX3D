#include "stdafx.h"
#include "AnimationDemo.h"

void AnimationDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(19, 0, 0);
	Context::Get()->GetCamera()->Position(0, 2, -4);

	shader = new Shader(L"19_Animation.fxo");

	Kachujin();

	sky = new CubeSky(L"Environment/Mountain1024.dds");
	
	planeShader = new Shader(L"15_Mesh.fxo");
	plane = new MeshGrid(planeShader, 6, 6);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}

void AnimationDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(kachujin);
	
	SafeDelete(sky);
	SafeDelete(planeShader);
	SafeDelete(plane);
}

void AnimationDemo::Update()
{
	//Lambert
	{
		static Vector3 LightDirection = Vector3(-1, -1, +1);
		ImGui::SliderFloat3("Light Direction", LightDirection, -1, +1);
		shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
		planeShader->AsVector("LightDirection")->SetFloatVector(LightDirection);
	}

	//Clip Test
	{
		static int clip = 0;
		static float speed = 1.f;
		static float takeTime = 0.1f;

		static bool bBlendMode = false;
		static float blendAlpha = 0.f;

		ImGui::Checkbox("BlendMode", &bBlendMode);
		
		//Tweening
		if (bBlendMode == false)
		{
			ImGui::SliderInt("Clip", &clip, 0, kachujin->GetModel()->ClipCount() - 1);

			const char* clipName[] = { "Idle", "Walk", "Run", "Slash", "Dance" };
			ImGui::Text("%s", clipName[clip]);
			ImGui::SliderFloat("Speed", &speed, 0.1f, 5.f);
			ImGui::SliderFloat("TakeTime", &takeTime, 0.1f, 5.f);

			if (ImGui::Button("Apply"))
				kachujin->PlayTweenMode(clip, speed, takeTime);
		}
		//Blending
		else
		{
			ImGui::SliderFloat("BlendAlpha", &blendAlpha, 0.f, 2.f);
			kachujin->SetBlendAlpha(blendAlpha);

			if (ImGui::Button("Apply"))
				kachujin->PlayBlendMode(0, 1, 2);
		}
		
	}

	sky->Update();
	plane->Update();

	if (kachujin != nullptr)
		kachujin->Update();
}

void AnimationDemo::Render()
{
	sky->Render();
	plane->Render();

	static bool bWire = false;
	ImGui::Checkbox("Wire Frame", &bWire);
	
	if (kachujin != nullptr)
	{
		kachujin->Pass(bWire ? 1 : 0);
		kachujin->Render();
	}

}

void AnimationDemo::Kachujin()
{
	kachujin = new ModelAnimator(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");
	kachujin->ReadClip(L"Kachujin/Idle");
	kachujin->ReadClip(L"Kachujin/Walk");
	kachujin->ReadClip(L"Kachujin/Run");
	kachujin->ReadClip(L"Kachujin/Slash");
	kachujin->ReadClip(L"Kachujin/Uprock");

	kachujin->GetTransform()->Position(0, 0, 0);
	kachujin->GetTransform()->Scale(0.01f, 0.01f, 0.01f);
}
