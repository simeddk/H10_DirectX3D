#include "stdafx.h"
#include "TerrainDemo.h"

void TerrainDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(6, 0, 0);
	Context::Get()->GetCamera()->Position(110, 50, -110);
	((Freedom*)Context::Get()->GetCamera())->Speed(50);

	shader = new Shader(L"13_Terrain.fxo");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/DarkDirt.png");
	terrain->LayerMap(L"Terrain/Cliff (Sandstone).jpg");
	terrain->AlphaMap(L"Terrain/Gray256.png");
}

void TerrainDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);
}

void TerrainDemo::Update()
{
	static UINT pass = shader->PassCount() - 1;
	ImGui::InputInt("Pass", (int*)&pass);
	pass %= shader->PassCount();
	terrain->Pass() = pass;

	static Vector3 lightDirection = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("LightDirection", lightDirection, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirection);

	static float tile = 1.f;
	ImGui::SliderFloat("Tile", &tile, 1.f, 10.f);
	terrain->Tile(tile);

	static float intensity = 4.f;
	ImGui::SliderFloat("Intensity", &intensity, 0, 10.f);
	terrain->AlphaIntensity(intensity);

	terrain->Update();
}

void TerrainDemo::Render()
{
	terrain->Render();
}

