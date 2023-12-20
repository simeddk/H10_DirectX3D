#include "stdafx.h"
#include "MeshDemo.h"

void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 49, -108);

	shader = new Shader(L"15_Mesh.fxo");

	CreateMesh();
}

void MeshDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(quad);
	SafeDelete(grid);
	SafeDelete(cube);

	for (UINT i = 0; i < 10; i++)
	{
		SafeDelete(cylinders[i]);
		SafeDelete(spheres[i]);
	}
}

void MeshDemo::Update()
{
	static Vector3 lightDirectoin = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("LightDirection", lightDirectoin, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirectoin);

	quad->Update();
	grid->Update();
	cube->Update();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Update();
		spheres[i]->Update();
	}
}

void MeshDemo::Render()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	quad->Pass(bWireFrame ? 1 : 0);
	quad->Render();

	grid->Pass(bWireFrame ? 1 : 0);
	grid->Render();

	cube->Pass(bWireFrame ? 1 : 0);
	cube->Render();

	for (UINT i = 0; i < 10; i++)
	{
		cylinders[i]->Pass(bWireFrame ? 1 : 0);
		cylinders[i]->Render();

		spheres[i]->Pass(bWireFrame ? 1 : 0);
		spheres[i]->Render();
	}
}

void MeshDemo::CreateMesh()
{
	quad = new MeshQuad(shader);
	quad->GetTransform()->Scale(5, 5, 1);
	quad->DiffuseMap(L"Box.png");

	grid = new MeshGrid(shader, 2.5f, 2.5f);
	grid->GetTransform()->Scale(12, 1, 12);
	grid->DiffuseMap(L"Floor.png");

	cube = new MeshCube(shader);
	cube->GetTransform()->Position(0, 5, 0);
	cube->GetTransform()->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	for (UINT i = 0; i < 5; i++)
	{
		//Even - Cylinders
		cylinders[i * 2 + 0] = new MeshCylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 0]->GetTransform()->Position(-30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 0]->DiffuseMap(L"Bricks.png");

		//Odd - Cylinders
		cylinders[i * 2 + 1] = new MeshCylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 1]->GetTransform()->Position(+30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		cylinders[i * 2 + 1]->DiffuseMap(L"Bricks.png");

		//Even - Spheres
		spheres[i * 2 + 0] = new MeshSphere(shader, 0.5f);
		spheres[i * 2 + 0]->GetTransform()->Position(-30, 15.5f, (float)i * 15.f - 15.f);
		spheres[i * 2 + 0]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 0]->DiffuseMap(L"Wall.png");

		//Odd - Spheres
		spheres[i * 2 + 1] = new MeshSphere(shader, 0.5f);
		spheres[i * 2 + 1]->GetTransform()->Position(+30, 15.5f, (float)i * 15.f - 15.f);
		spheres[i * 2 + 1]->GetTransform()->Scale(5, 5, 5);
		spheres[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}
