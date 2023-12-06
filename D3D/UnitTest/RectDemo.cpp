#include "stdafx.h"
#include "RectDemo.h"

void RectDemo::Initialize()
{
	shader = new Shader(L"05_Rect.fxo");

	//Vertex Data
	{
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
}

void RectDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
}

void RectDemo::Update()
{
	
}

void RectDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static bool bWire = false;
	ImGui::Checkbox("WireFrame", &bWire);

	shader->Draw(0, bWire ? 1 : 0, 6);
}
