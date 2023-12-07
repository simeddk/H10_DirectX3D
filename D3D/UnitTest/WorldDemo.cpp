#include "stdafx.h"
#include "WorldDemo.h"

void WorldDemo::Initialize()
{
	shader = new Shader(L"06_World.fxo");

	//Vertex Data
	{
		/*vertices[0].Position = Vector3(-0.5f + 0.5f, -0.5f + 0.5f, 0);
		vertices[1].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0);
		vertices[2].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0);

		vertices[3].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0);
		vertices[4].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0);
		vertices[5].Position = Vector3(+0.5f + 0.5f, +0.5f + 0.5f, 0);*/

		vertices[0].Position = Vector3(-0.5f, -0.5f, 0);
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0);

		vertices[3].Position = Vector3(+0.5f, -0.5f, 0);
		vertices[4].Position = Vector3(-0.5f, +0.5f, 0);
		vertices[5].Position = Vector3(+0.5f, +0.5f, 0);
	}

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subRerouce = { 0 };
		subRerouce.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subRerouce, &vertexBuffer));
	}

	D3DXMatrixIdentity(&world);
}

void WorldDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
}

void WorldDemo::Update()
{
#ifdef MatrixMember
	if (Keyboard::Get()->Press(VK_LSHIFT))
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world._11 += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world._11 -= 2.f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world._22 += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world._22 -= 2.f * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			world._41 += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			world._41 -= 2.f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			world._42 += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			world._42 -= 2.f * Time::Delta();
	}
#endif

	static Vector3 position = Vector3(0, 0, 0);
	static Vector3 scale = Vector3(1, 1, 1);

	if (Keyboard::Get()->Press(VK_LSHIFT))
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			scale.x += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			scale.x -= 2.f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			scale.y += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			scale.y -= 2.f * Time::Delta();
	}
	else
	{
		if (Keyboard::Get()->Press(VK_RIGHT))
			position.x += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_LEFT))
			position.x -= 2.f * Time::Delta();

		if (Keyboard::Get()->Press(VK_UP))
			position.y += 2.f * Time::Delta();
		else if (Keyboard::Get()->Press(VK_DOWN))
			position.y -= 2.f * Time::Delta();
	}

	Matrix S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);

	world = S * T;

	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void WorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static bool bWire = false;
	ImGui::Checkbox("WireFrame", &bWire);

	shader->Draw(0, bWire ? 1 : 0, 6);
}
