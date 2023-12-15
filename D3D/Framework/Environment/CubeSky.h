#pragma once

class CubeSky
{
public:
	CubeSky(wstring ddsFile);
	~CubeSky();

	void Update();
	void Render();

public:
	void Pass(UINT val) { sphere->Pass(val); }
	Shader* GetShader() { return shader; }

private:
	Shader* shader;

	MeshSphere* sphere;

	ID3D11ShaderResourceView* srv;
	ID3DX11EffectShaderResourceVariable* sSrv;
};
