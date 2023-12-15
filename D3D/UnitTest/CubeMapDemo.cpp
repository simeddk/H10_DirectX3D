#include "stdafx.h"
#include "CubeMapDemo.h"
#include "Objects/CubeMap.h"

void CubeMapDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(23, 0, 0);
	Context::Get()->GetCamera()->Position(0, 49, -108);
	((Freedom*)Context::Get()->GetCamera())->Speed(50);

	shader = new Shader(L"15_Mesh.fxo");

	CreateMesh();

	cubeMapShader = new Shader(L"16_CubeMap.fxo");
	cubeMap = new CubeMap(cubeMapShader, EMeshType::Cube);
	cubeMap->Texture(L"Environment/SnowCube1024.dds");
	cubeMap->Position(0, 20, 0);
	cubeMap->Scale(10, 10, 10);
}

void CubeMapDemo::Destroy()
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

	SafeDelete(cubeMapShader);
	SafeDelete(cubeMap);
}

void CubeMapDemo::Update()
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

	cubeMap->Update();
}

void CubeMapDemo::Render()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	static int pass = 0;
	ImGui::SliderInt("CubeMap Pass", &pass, 0, 1);
	cubeMap->Pass(pass);

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

	cubeMap->Pass(bWireFrame ? 2 : 0);
	cubeMap->Render();
}

void CubeMapDemo::CreateMesh()
{
	quad = new MeshQuad(shader);
	quad->Scale(5, 5, 1);
	quad->DiffuseMap(L"Box.png");

	grid = new MeshGrid(shader, 2.5f, 2.5f);
	grid->Scale(12, 1, 12);
	grid->DiffuseMap(L"Floor.png");

	cube = new MeshCube(shader);
	cube->Position(0, 5, 0);
	cube->Scale(20, 10, 20);
	cube->DiffuseMap(L"Stones.png");

	for (UINT i = 0; i < 5; i++)
	{
		//Even - Cylinders
		cylinders[i * 2 + 0] = new MeshCylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 0]->Position(-30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 0]->Scale(5, 5, 5);
		cylinders[i * 2 + 0]->DiffuseMap(L"Bricks.png");

		//Odd - Cylinders
		cylinders[i * 2 + 1] = new MeshCylinder(shader, 0.3f, 0.5f, 3.f, 20, 20);
		cylinders[i * 2 + 1]->Position(+30, 6, (float)i * 15.f - 15.f);
		cylinders[i * 2 + 1]->Scale(5, 5, 5);
		cylinders[i * 2 + 1]->DiffuseMap(L"Bricks.png");

		//Even - Spheres
		spheres[i * 2 + 0] = new MeshSphere(shader, 0.5f);
		spheres[i * 2 + 0]->Position(-30, 15.5f, (float)i * 15.f - 15.f);
		spheres[i * 2 + 0]->Scale(5, 5, 5);
		spheres[i * 2 + 0]->DiffuseMap(L"Wall.png");

		//Odd - Spheres
		spheres[i * 2 + 1] = new MeshSphere(shader, 0.5f);
		spheres[i * 2 + 1]->Position(+30, 15.5f, (float)i * 15.f - 15.f);
		spheres[i * 2 + 1]->Scale(5, 5, 5);
		spheres[i * 2 + 1]->DiffuseMap(L"Wall.png");
	}
}
