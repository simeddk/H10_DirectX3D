#include "stdafx.h"
#include "MeshDemo.h"

void MeshDemo::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 36, -85);
	Context::Get()->GetCamera()->RotationDegree(20, 0, 0);

	shader = new Shader(L"15_Mesh.fxo");

	CreateMesh();
}

void MeshDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(quad);
}

void MeshDemo::Update()
{
	static Vector3 lightDirectoin = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("LightDirection", lightDirectoin, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirectoin);

	quad->Update();
}

void MeshDemo::Render()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);

	quad->Pass(bWireFrame ? 1 : 0);
	quad->Render();
}

void MeshDemo::CreateMesh()
{
	quad = new MeshQuad(shader);
	quad->DiffuseMap(L"Box.png");
}
