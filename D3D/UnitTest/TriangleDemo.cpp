#include "stdafx.h"
#include "TriangleDemo.h"

void TriangleDemo::Initialize()
{
	shader = new Shader(L"04_Triangle.fx");

	//Vertex Data 1
	{
		vertices[0].Position = Vector3(-0.5f, +0.0f, 0);
		vertices[1].Position = Vector3(+0.0f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, +0.0f, 0);
	}

	//Vertex Data 2
	{
		vertices2[0].Position = Vector3(-0.5f, +0.0f - 0.5f, 0);
		vertices2[1].Position = Vector3(+0.0f, +0.5f - 0.5f, 0);
		vertices2[2].Position = Vector3(+0.5f, +0.0f - 0.5f, 0);
	}

	//Create VertexBuffer 1
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subRerouce = { 0 };
		subRerouce.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subRerouce, &vertexBuffer));
	}

	//Create VertexBuffer 2
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 3;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subRerouce = { 0 };
		subRerouce.pSysMem = vertices2;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subRerouce, &vertexBuffer2));
	}
	
}

void TriangleDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
	SafeRelease(vertexBuffer2);
}

void TriangleDemo::Update()
{
}

void TriangleDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 3);

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer2, &stride, &offset);
	shader->Draw(0, 0, 3);
}
