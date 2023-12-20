#pragma once

enum class EMeshType { Cube, Cylinder, Sphere };

class CubeMap
{
public:
	CubeMap(Shader* shader, EMeshType meshType);
	~CubeMap();

	void Texture(wstring ddsFile);

	void Update();
	void Render();

public:
	void Pass(UINT val);

	Transform* GetTransform() { return mesh->GetTransform(); }

private:
	Shader* shader;

	Mesh* mesh;

	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSrv;
};