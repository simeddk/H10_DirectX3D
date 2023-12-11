#pragma once

#include "Systems/IExecute.h"

class AddressDemo : public IExecute
{
public:
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update() override;
	virtual void PreRender() override {};
	virtual void Render() override;
	virtual void PostRender() override {};
	virtual void ResizeScreen() override {};

private:
	void OnOpenCompleted(wstring path);

private:
	struct Vertex
	{
		Vector3 Position;
		Vector2 Uv;
	};

private:
	Shader* shader = nullptr;

	Vertex* vertices;
	ID3D11Buffer* vertexBuffer;

	UINT* indices;
	ID3D11Buffer* indexBuffer;

	Matrix world;

	Texture* texture = nullptr;
};