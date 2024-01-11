#include "stdafx.h"
#include "ModelDemo.h"

void ModelDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(12, 0, 0);
	Context::Get()->GetCamera()->Position(1, 5, -23);

	shader = new Shader(L"18_Model.fxo");

	Tank();
	Kachujin();
	Tower();
	Airplane();

	sky = new CubeSky(L"Environment/Mountain1024.dds");
	
	planeShader = new Shader(L"15_Mesh.fxo");
	plane = new MeshGrid(planeShader, 6, 6);
	plane->GetTransform()->Scale(12, 1, 12);
	plane->DiffuseMap(L"Floor.png");
}

void ModelDemo::Destroy()
{
	SafeDelete(shader);

	SafeDelete(tank);
	SafeDelete(kachujin);
	SafeDelete(tower);
	SafeDelete(airplane);
	
	SafeDelete(sky);
	SafeDelete(planeShader);
	SafeDelete(plane);
}

void ModelDemo::Update()
{
	static Vector3 LightDirection = Vector3(-1, -1, +1);
	ImGui::SliderFloat3("Light Direction", LightDirection, -1, +1);
	shader->AsVector("LightDirection")->SetFloatVector(LightDirection);
	planeShader->AsVector("LightDirection")->SetFloatVector(LightDirection);

	sky->Update();
	plane->Update();

	if (tank != nullptr)
		tank->Update();

	if (kachujin != nullptr)
		kachujin->Update();

	if (tower != nullptr)
		tower->Update();

	if (airplane != nullptr)
		airplane->Update();
}

void ModelDemo::Render()
{
	sky->Render();
	plane->Render();

	static bool bWire = false;
	ImGui::Checkbox("Wire Frame", &bWire);

	if (tank != nullptr)
	{
		tank->Pass(bWire ? 1 : 0);
		tank->Render();
	}

	if (kachujin != nullptr)
	{
		kachujin->Pass(bWire ? 1 : 0);
		kachujin->Render();
	}

	if (tower != nullptr)
	{
		tower->Pass(bWire ? 1 : 0);
		tower->Render();
	}

	if (airplane != nullptr)
	{
		airplane->Pass(bWire ? 1 : 0);
		airplane->Render();
	}
}

void ModelDemo::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMesh(L"Tank/Tank");
	tank->ReadMaterial(L"Tank/Tank");
}

void ModelDemo::Kachujin()
{
	kachujin = new ModelRender(shader);
	kachujin->ReadMesh(L"Kachujin/Mesh");
	kachujin->ReadMaterial(L"Kachujin/Mesh");

	kachujin->GetTrasnform()->Position(5, 0, 0);
	kachujin->GetTrasnform()->Scale(0.01f, 0.01f, 0.01f);
}

void ModelDemo::Tower()
{
	tower = new ModelRender(shader);
	tower->ReadMesh(L"Tower/Tower");
	tower->ReadMaterial(L"Tower/Tower");
	
	tower->GetTrasnform()->Position(-5, 0, 0);
	tower->GetTrasnform()->Scale(0.01f, 0.01f, 0.01f);
}

void ModelDemo::Airplane()
{
	airplane = new ModelRender(shader);
	airplane->ReadMesh(L"B787/Airplane");
	airplane->ReadMaterial(L"B787/Airplane");
	
	airplane->GetTrasnform()->Position(-10, 0, 0);
	airplane->GetTrasnform()->Scale(0.001f, 0.001f, 0.001f);
}
