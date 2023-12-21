#pragma once

class Terrain : public Renderer
{
public:
	Terrain(Shader* shader, wstring heightMapFile);
	~Terrain();

	void Update();
	void Render();

	void VisibleNormal();

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

private:
	struct VertexTerrain
	{
		Vector3 Position = Vector3(0, 0, 0);
		Vector3 Normal = Vector3(0, 0, 0);
		Vector2 Uv = Vector2(0, 0);
	};

private:
	UINT width, height;

	VertexTerrain* vertices;
	UINT* indices;

	Texture* heightMap;
	Texture* baseMap = nullptr;
	Texture* layerMap = nullptr;
	Texture* alphaMap = nullptr;

	float tile = 1.f;
	float alphaIntensity = 1.f;
};