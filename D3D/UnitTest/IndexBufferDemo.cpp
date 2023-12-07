#include "stdafx.h"
#include "IndexBufferDemo.h"

void IndexBufferDemo::Initialize()
{
	shader = new Shader(L"07_Index.fxo");

	//Vertex Data
	{
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0);
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0);
		vertices[3].Position = Vector3(+0.5f, +0.5f, 0);
	}

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subRerouce = { 0 };
		subRerouce.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subRerouce, &vertexBuffer));
	}

	//Index Data
	{
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;

		indices[3] = 2;
		indices[4] = 1;
		indices[5] = 3;
	}

	//Create IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA subRerouce = { 0 };
		subRerouce.pSysMem = indices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subRerouce, &indexBuffer));
	}

	D3DXMatrixIdentity(&world);
}

void IndexBufferDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);
}

void IndexBufferDemo::Update()
{
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void IndexBufferDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static bool bWire = false;
	ImGui::Checkbox("WireFrame", &bWire);

	shader->DrawIndexed(0, bWire ? 1 : 0, 6);
}
