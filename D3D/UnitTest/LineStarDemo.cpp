#include "stdafx.h"
#include "LineStarDemo.h"

void LineStarDemo::Initialize()
{
	shader = new Shader(L"03_LineStar.fx");

	//Vertex Data
	{
		vertices[0].Position = Vector3(+0.0f, +0.75f, +0.0f);
		vertices[1].Position = Vector3(-0.5f, -0.5f, +0.0f);
		vertices[2].Position = Vector3(+0.5f, +0.25f, +0.0f);
		vertices[3].Position = Vector3(-0.5f, +0.25f, +0.0f);
		vertices[4].Position = Vector3(+0.5f, -0.5f, +0.0f);
		vertices[5].Position = Vector3(+0.0f, +0.75f, +0.0f);

		vertices[0].Color = Color(1, 0, 0, 1);
		vertices[1].Color = Color(0, 1, 0, 1);
		vertices[2].Color = Color(0, 0, 1, 1);
		vertices[3].Color = Color(0, 1, 1, 1);
		vertices[4].Color = Color(1, 0, 1, 1);
		vertices[5].Color = Color(1, 0, 0, 1);
	}

	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//desc.Usage = D3D11_USAGE_DEFAULT; <- CPU Write X, GPU Write O		(Speed : 3)
		//desc.Usage = D3D11_USAGE_DYNAMIC; <- CPU Write O, GPU Write X		(Speed : 2)
		//desc.Usage = D3D11_USAGE_STAGING; <- CPU Write O, GPU Write O		(Speed : 4)
		//desc.Usage = D3D11_USAGE_IMMUTABLE; <- CPU Write X, GPU Write X	(Speed : 1)
		//-> D3D::GetDC()->UpdateSubresource() : GPU Wirte Only
		//-> D3D::GetDC()->Map()				: CPU Wirte Only
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA subRerouce = { 0 };
		subRerouce.pSysMem = vertices;

		Check(D3D::GetDevice()->CreateBuffer(&desc, &subRerouce, &vertexBuffer));
	}
}

void LineStarDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void LineStarDemo::Update()
{
}

void LineStarDemo::Render()
{
	static int pass = 0;
	ImGui::SliderInt("Pass", &pass, 0, 1);

	if (pass == 0)
	{
		static Color vertexColor[6] = 
		{
			Color(1, 0, 0, 1),
			Color(0, 1, 0, 1),
			Color(0, 0, 1, 1),
			Color(1, 0, 1, 1),
			Color(1, 1, 0, 1),
			Color(0, 1, 1, 1),
		};

		ImGui::ColorEdit3("Vertex0", vertexColor[0]);
		ImGui::ColorEdit3("Vertex1", vertexColor[1]);
		ImGui::ColorEdit3("Vertex2", vertexColor[2]);
		ImGui::ColorEdit3("Vertex3", vertexColor[3]);
		ImGui::ColorEdit3("Vertex4", vertexColor[4]);
		ImGui::ColorEdit3("Vertex5", vertexColor[5]);

		vertices[0].Color = vertexColor[0];
		vertices[1].Color = vertexColor[1];
		vertices[2].Color = vertexColor[2];
		vertices[3].Color = vertexColor[3];
		vertices[4].Color = vertexColor[4];
		vertices[5].Color = vertexColor[5];

		static Vector2 position = Vector2(-0.5f, -0.5f);
		ImGui::SliderFloat2("Vertex1 Position", position, -1, +1);
		vertices[1].Position = Vector3(position.x, position.y, 0);
	}

	if (pass == 1)
	{
		static Color starColor = Color(1, 1, 1, 1);
		ImGui::ColorEdit3("Color", starColor);
		shader->AsVector("StarColor")->SetFloatVector(starColor);
	}

#ifdef UpdateSubresource
	//1. UpdateSubresource(GPU Write Access)
	D3D::GetDC()->UpdateSubresource
	(
		vertexBuffer,
		0,
		nullptr,
		vertices,
		sizeof(Vertex) * 6, 
		0
	);
#endif

	//2. Map(CPU Wirte Access)
	D3D11_MAPPED_SUBRESOURCE subResource;
	D3D::GetDC()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		memcpy(subResource.pData, vertices, sizeof(Vertex) * 6);
	}
	D3D::GetDC()->Unmap(vertexBuffer, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	shader->Draw(0, pass, 6);
}
