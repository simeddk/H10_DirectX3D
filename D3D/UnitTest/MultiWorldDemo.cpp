#include "stdafx.h"
#include "MultiWorldDemo.h"

void MultiWorldDemo::Initialize()
{
	shader = new Shader(L"06_World.fxo");

	//Vertex Data
	{
		vertices[0].Position = Vector3(-0.5f + 0.5f, -0.5f + 0.5f, 0);
		vertices[1].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0);
		vertices[2].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0);

		vertices[3].Position = Vector3(+0.5f + 0.5f, -0.5f + 0.5f, 0);
		vertices[4].Position = Vector3(-0.5f + 0.5f, +0.5f + 0.5f, 0);
		vertices[5].Position = Vector3(+0.5f + 0.5f, +0.5f + 0.5f, 0);

		/*vertices[0].Position = Vector3(-0.5f, -0.5f, 0);
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0);
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0);

		vertices[3].Position = Vector3(+0.5f, -0.5f, 0);
		vertices[4].Position = Vector3(-0.5f, +0.5f, 0);
		vertices[5].Position = Vector3(+0.5f, +0.5f, 0);*/
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

	for (UINT i = 0; i < 10; i++)
	{
		Vector3 scale = Math::RandomVec3(0.5f, 1.2f);
		Vector3 position = Math::RandomVec3(-3.f, +3.f);
		position.z = 0.f;

		Matrix S, T;
		D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
		D3DXMatrixTranslation(&T, position.x, position.y, position.z);

		worlds[i] = S * T;
		colors[i] = Math::RandomColor4();
	}
}

void MultiWorldDemo::Destroy()
{
	SafeDelete(shader);

	SafeRelease(vertexBuffer);
}

void MultiWorldDemo::Update()
{
	//Select World
	static UINT index = 0;
	{
		if (Keyboard::Get()->Down(VK_ADD))
			index++;
		if (Keyboard::Get()->Down(VK_SUBTRACT))
			index--;

		index = Math::Clamp<UINT>(index, 0, ARRAYSIZE(worlds) - 1);
	}

	//Print Debug
	{
		ImVec4 textColor = ImVec4(colors[index].r, colors[index].g, colors[index].b, colors[index].a);;
		ImGui::TextColored(textColor, "Index : %d", index);

		Vector2 position = Vector2(worlds[index]._41, worlds[index]._42);
		Vector2 scale = Vector2(worlds[index]._11, worlds[index]._22);
		ImGui::TextColored(textColor, "Position : %.2f, %.2f", position.x, position.y);
		ImGui::TextColored(textColor, "Scale : %.2f, %.2f", scale.x, scale.y);
	}

	//Set Transform
	{
		//Scale
		if (Keyboard::Get()->Press(VK_LSHIFT))
		{
			if (Keyboard::Get()->Press(VK_RIGHT))
				worlds[index]._11 += 5.f * Time::Delta();
			else if (Keyboard::Get()->Press(VK_LEFT))
				worlds[index]._11 -= 5.f * Time::Delta();

			if (Keyboard::Get()->Press(VK_UP))
				worlds[index]._22 += 5.f * Time::Delta();
			else if (Keyboard::Get()->Press(VK_DOWN))
				worlds[index]._22 -= 5.f * Time::Delta();
		}
		//Translation
		else
		{
			if (Keyboard::Get()->Press(VK_RIGHT))
				worlds[index]._41 += 5.f * Time::Delta();
			else if (Keyboard::Get()->Press(VK_LEFT))
				worlds[index]._41 -= 5.f * Time::Delta();

			if (Keyboard::Get()->Press(VK_UP))
				worlds[index]._42 += 5.f * Time::Delta();
			else if (Keyboard::Get()->Press(VK_DOWN))
				worlds[index]._42 -= 5.f * Time::Delta();
		}

		//Rotation
		if (Mouse::Get()->Press(1))
		{
			for (auto& world : worlds)
			{
				Vector3 scale, rotation, position;
				Math::MatrixDecompose(world, scale, rotation, position);

				rotation.z += 10.f * Time::Delta();
				
				Matrix S, R, T;
				D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
				D3DXMatrixRotationZ(&R, rotation.z);
				D3DXMatrixTranslation(&T, position.x, position.y, position.z);

				world = S * R * T;
			}
		}
	}

	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void MultiWorldDemo::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	static bool bWire = false;
	ImGui::Checkbox("WireFrame", &bWire);

	for (UINT i = 0; i < 10; i++)
	{
		shader->AsVector("Color")->SetFloatVector(colors[i]);
		shader->AsMatrix("World")->SetMatrix(worlds[i]);
		shader->Draw(0, bWire ? 1 : 0, 6);
	}

	
}
