#include "stdafx.h"
#include "LineTopologyDemo.h"

void LineTopologyDemo::Initialize()
{
	shader = new Shader(L"02_LineColor.fx");

	//Vertex Data
	{
		vertices[0].Position = Vector3(0, 0, 0);
		vertices[0].Color = Color(1, 0, 0, 1);

		vertices[1].Position = Vector3(1, 0, 0);
		vertices[1].Color = Color(0, 1, 0, 1);

		vertices[2].Position = Vector3(0, 0.5f, 0);
		vertices[2].Color = Color(1, 0, 0, 1);

		vertices[3].Position = Vector3(1, 0.5f, 0);
		vertices[3].Color = Color(0, 1, 0, 1);

		vertices[4].Position = Vector3(0, -0.5f, 0);
		vertices[4].Color = Color(1, 0, 0, 1);

		vertices[5].Position = Vector3(1, -0.5f, 0);
		vertices[5].Color = Color(0, 1, 0, 1);
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

void LineTopologyDemo::Destroy()
{
	SafeDelete(shader);
	SafeRelease(vertexBuffer);
}

void LineTopologyDemo::Update()
{
}

void LineTopologyDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	//Topology Test
	{
		static bool bStrip = false;
		ImGui::Checkbox("Strip Mode", &bStrip);

		D3D11_PRIMITIVE_TOPOLOGY topology[2] =
		{
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP
		};

		D3D::GetDC()->IASetPrimitiveTopology(bStrip ? topology[1] : topology[0]);
	}

	//StartVertextLocation Test
	static UINT startLocation = 0;
	{
		ImGui::SliderInt("Start Verexe", (int*)&startLocation, 0, 6);
	}

	shader->Draw(0, 0, 6, startLocation);
}
