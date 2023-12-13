#include "stdafx.h"
#include "GetHeightDemo.h"

void GetHeightDemo::Initialize()
{
	Context::Get()->GetCamera()->RotationDegree(6, 0, 0);
	Context::Get()->GetCamera()->Position(110, 50, -110);
	((Freedom*)Context::Get()->GetCamera())->Speed(50);

	shader = new Shader(L"13_Terrain.fxo");
	terrain = new Terrain(shader, L"Terrain/Gray256.png");
	terrain->BaseMap(L"Terrain/DarkDirt.png");
	terrain->LayerMap(L"Terrain/Cliff (Sandstone).jpg");
	terrain->AlphaMap(L"Terrain/Gray256.png");
	terrain->AlphaIntensity(5.f);
	terrain->Tile(5.f);

	triShader = new Shader(L"14_TwoSide.fxo");
	Vertex vertices[] =	{ Vector3(0, 1, 0), Vector3(-1, 0, 0), Vector3(1, 0, 0) };

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subResrouce = { 0 };
		subResrouce.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subResrouce, &vertexBuffer));
	}


}

void GetHeightDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(terrain);

	SafeDelete(triShader);
	SafeRelease(vertexBuffer);
}

void GetHeightDemo::Update()
{
	static bool bWireFrame = false;
	ImGui::Checkbox("WireFrame", &bWireFrame);
	terrain->Pass() = bWireFrame ? 1 : 0;

	static Vector3 lightDirection = Vector3(-1, -1, 1);
	ImGui::SliderFloat3("LightDirection", lightDirection, -1, 1);
	shader->AsVector("LightDirection")->SetFloatVector(lightDirection);

	terrain->Update();

	if (Keyboard::Get()->Press(VK_RIGHT))
		position.x += 20.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_LEFT))
		position.x -= 20.f * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		position.z += 20.f * Time::Delta();
	else if (Keyboard::Get()->Press(VK_DOWN))
		position.z -= 20.f * Time::Delta();

	//position.y = terrain->GetHeightByUV(position) + 1.f;
	position.y = terrain->GetHeightByRaycast(position) + 1.f;

	Matrix R, T;
	D3DXMatrixRotationX(&R, Math::PI);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	Matrix world = R * T;

	triShader->AsMatrix("World")->SetMatrix(world);
	triShader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	triShader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());

}

void GetHeightDemo::Render()
{
	terrain->Render();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	triShader->Draw(0, 0, 3);

	string str = to_string(position.x) + ", " + to_string(position.y) + ", " + to_string(position.z);
	Gui::Get()->RenderText(5, 60, str);
}

