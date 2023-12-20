#include "Framework.h"
#include "CubeMap.h"

CubeMap::CubeMap(Shader* shader, EMeshType meshType)
	: shader(shader)
{
	switch (meshType)
	{
		case EMeshType::Cube:
		{
			mesh = new MeshCube(shader);
		}
		break;

		case EMeshType::Cylinder:
		{
			mesh = new MeshCylinder(shader, 0.5f, 1.25f, 20, 20);
		}
		break;

		case EMeshType::Sphere:
		{
			mesh = new MeshSphere(shader, 0.5f);
		}
		break;
	}

	sSrv = shader->AsSRV("CubeMap");
}

CubeMap::~CubeMap()
{
	SafeDelete(mesh);
	
	SafeRelease(srv);
}

void CubeMap::Texture(wstring ddsFile)
{
	ddsFile = L"../../_Textures/" + ddsFile;

	Check(D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(),
		ddsFile.c_str(),
		nullptr,
		nullptr,
		&srv,
		nullptr
	));
}

void CubeMap::Update()
{
	mesh->Update();
}

void CubeMap::Render()
{
	sSrv->SetResource(srv);

	mesh->Render();
}

void CubeMap::Pass(UINT val)
{
	mesh->Pass(val);
}
