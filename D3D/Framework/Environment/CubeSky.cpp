#include "Framework.h"
#include "CubeSky.h"

CubeSky::CubeSky(wstring ddsFile)
{
	shader = new Shader(L"17_CubeSky.fxo");
	sphere = new MeshSphere(shader, 0.5f);

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

	sSrv = shader->AsSRV("SkyMap");
}

CubeSky::~CubeSky()
{
	SafeDelete(shader);
	SafeDelete(sphere);

	SafeRelease(srv);
}

void CubeSky::Update()
{
	Vector3 camperaPosition;
	Context::Get()->GetCamera()->Position(&camperaPosition);
	sphere->Position(camperaPosition);

	sphere->Update();
}

void CubeSky::Render()
{
	sSrv->SetResource(srv);

	sphere->Render();
}
