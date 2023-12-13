#pragma once

class Terrain
{
public:
	Terrain(Shader* shader, wstring heightMapFile);
	~Terrain();

	void Update();
	void Render();

	void VisibleNormal();

	UINT& Pass() { return pass; }

	void BaseMap(wstring path);
	void LayerMap(wstring path);
	void AlphaMap(wstring path);
	void Tile(float tile) { this->tile = tile; }
	void AlphaIntensity(float intensity) { alphaIntensity = intensity; }

	float GetHeightByUV(Vector3& position);
	float GetHeightByRaycast(Vector3& position);

private:
	void CreateVertexData();
	void CreateIndexData();
	void CreateNormalData();
	void CreateBuffer();

private:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
	};

private:
	Shader* shader;
	UINT pass = 0;

	UINT width, height;

	UINT vertexCount;
	VertexTerrain* vertices;
	ID3D11Buffer* vertexBuffer;

	UINT indexCount;
	UINT* indices;
	ID3D11Buffer* indexBuffer;

	Matrix world;

	Texture* heightMap;
	Texture* baseMap = nullptr;
	Texture* layerMap = nullptr;
	Texture* alphaMap = nullptr;

	float tile = 1.f;
	float alphaIntensity = 1.f;
};