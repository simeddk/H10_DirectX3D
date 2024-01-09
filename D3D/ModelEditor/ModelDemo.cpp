#include "stdafx.h"
#include "ModelDemo.h"

void ModelDemo::Initialize()
{
	shader = new Shader(L"18_Model.fxo");

	Tank();
}

void ModelDemo::Destroy()
{
	SafeDelete(shader);
	SafeDelete(tank);
}

void ModelDemo::Update()
{
	if (tank != nullptr)
		tank->Update();
}

void ModelDemo::Render()
{
	//UnresolvedMergeConflict. Lambert, Test....
	if (tank != nullptr)
	{
		tank->Pass(1);
		tank->Render();
	}
}

void ModelDemo::Tank()
{
	tank = new ModelRender(shader);
	tank->ReadMesh(L"Tank/Tank");
}
