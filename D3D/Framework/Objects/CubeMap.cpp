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

void CubeMap::Position(float x, float y, float z)
{
	mesh->Position(x, y, z);
}

void CubeMap::Position(Vector3& vec)
{
	mesh->Position(vec);
}

void CubeMap::Position(Vector3* vec)
{
	Vector3 position;
	mesh->Position(&position);

	*vec = position;
}

void CubeMap::Scale(float x, float y, float z)
{
	mesh->Scale(x, y, z);
}

void CubeMap::Scale(Vector3& vec)
{
	mesh->Scale(vec);
}

void CubeMap::Scale(Vector3* vec)
{
	Vector3 scale;
	mesh->Scale(&scale);

	*vec = scale;
}

void CubeMap::Rotation(float x, float y, float z)
{
	mesh->Rotation(x, y, z);
}

void CubeMap::Rotation(Vector3& vec)
{
	mesh->Rotation(vec);
}

void CubeMap::Rotation(Vector3* vec)
{
	Vector3 rotation;
	mesh->Rotation(&rotation);

	*vec = rotation;
}

void CubeMap::RotationDegree(float x, float y, float z)
{
	mesh->RotationDegree(x, y, z);
}

void CubeMap::RotationDegree(Vector3& vec)
{
	mesh->RotationDegree(vec);
}

void CubeMap::RotationDegree(Vector3* vec)
{
	Vector3 rotation;
	mesh->RotationDegree(&rotation);

	*vec = rotation;
}
