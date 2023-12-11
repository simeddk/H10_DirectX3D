#include "stdafx.h"
#include "TextureDemo.h"

void TextureDemo::Initialize()
{
	Context::Get()->GetCamera()->Position(0, 0, -5);
	Context::Get()->GetCamera()->RotationDegree(0, 0, 0);

	shader = new Shader(L"09_Texture.fxo");

	//Vertex Data
	{
		vertices = new Vertex[4];

		vertices[0].Position = Vector3(-0.5f, -0.5f, 0);
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0);
		vertices[3].Position = Vector3(+0.5f, +0.5f, 0);

		vertices[0].Uv = Vector2(0, 1);
		vertices[1].Uv = Vector2(0, 0);
		vertices[2].Uv = Vector2(1, 1);
		vertices[3].Uv = Vector2(1, 0);
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
	indices = new UINT[6]{ 0, 1, 2, 2, 1, 3 };
	
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

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	D3DXMatrixIdentity(&world);

	texture = new Texture(L"Box.png");
}

void TextureDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
	SafeRelease(indexBuffer);

	SafeDelete(texture);
}

void TextureDemo::Update()
{
	if (ImGui::Button("Load Image"))
	{
		function<void(wstring)> onOpenCompleted = bind(&TextureDemo::OnOpenCompleted, this, placeholders::_1);
		Path::OpenFileDialog(L"", Path::ImageFilter, L"../../_Textures/", onOpenCompleted, D3D::GetDesc().Handle);
	}

	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void TextureDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->AsSRV("DiffuseMap")->SetResource(texture->SRV());

	static UINT pass = 0;
	ImGui::SliderInt("Pass", (int*)&pass, 0, 1);

	shader->DrawIndexed(0, pass, 6);
}

void TextureDemo::OnOpenCompleted(wstring path)
{
	SafeDelete(texture);

	texture = new Texture(path);

	D3D11_SAMPLER_DESC;
}
