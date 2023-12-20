#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain(Shader* shader, wstring heightMapFile)
	: shader(shader)
{
	heightMap = new Texture(heightMapFile);

	CreateVertexData();
	CreateIndexData();
	CreateNormalData();
	
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(VertexTerrain));
	indexBuffer = new IndexBuffer(indices, indexCount);

	D3DXMatrixIdentity(&world);
}

Terrain::~Terrain()
{
	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(heightMap);
}

void Terrain::Update()
{
	shader->AsMatrix("World")->SetMatrix(world);
	shader->AsMatrix("View")->SetMatrix(Context::Get()->View());
	shader->AsMatrix("Projection")->SetMatrix(Context::Get()->Projection());
}

void Terrain::Render()
{
	VisibleNormal();

	if (baseMap != nullptr)
		shader->AsSRV("BaseMap")->SetResource(baseMap->SRV());

	if (layerMap != nullptr)
		shader->AsSRV("LayerMap")->SetResource(layerMap->SRV());

	if (alphaMap != nullptr)
		shader->AsSRV("AlphaMap")->SetResource(alphaMap->SRV());

	shader->AsScalar("Tile")->SetFloat(tile);
	shader->AsScalar("AlphaIntensity")->SetFloat(alphaIntensity);

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, indexCount);
}

void Terrain::VisibleNormal()
{
	static bool bVisibleNormal = false;
	ImGui::Checkbox("Visible Normal", &bVisibleNormal);

	static UINT interval = 3;
	ImGui::SliderInt("Interval", (int*)&interval, 1, 5);

	if (bVisibleNormal)
	{
		for (UINT z = 0; z < height; z += interval)
		{
			for (UINT x = 0; x < width; x += interval)
			{
				UINT index = width * z + x;

				Vector3 start = vertices[index].Position;
				Vector3 end = start + vertices[index].Normal;

				DebugLine::Get()->RenderLine(start, end, Color(1, 0, 0, 1));
			}
		}
	}
}

void Terrain::BaseMap(wstring path)
{
	SafeDelete(baseMap);

	baseMap = new Texture(path);
}

void Terrain::LayerMap(wstring path)
{
	SafeDelete(layerMap);

	layerMap = new Texture(path);
}

void Terrain::AlphaMap(wstring path)
{
	SafeDelete(alphaMap);

	alphaMap = new Texture(path);
}

float Terrain::GetHeightByUV(Vector3& position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width - 2) return FLT_MIN;
	if (z < 0 || z > height - 2) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 v[4];
	for (UINT i = 0; i < 4; i++)
		v[i] = vertices[index[i]].Position;

	float ddx = position.x - v[0].x;
	float ddz = position.z - v[0].z;

	Vector3 result;

	if (ddx + ddz <= 1.f)
	{
		result = v[0] + (v[2] - v[0]) * ddx + (v[1] - v[0]) * ddz;
	}
	else
	{
		ddx = 1.f - ddx;
		ddz = 1.f - ddz;

		result = v[3] + (v[1] - v[3]) * ddx + (v[2] - v[3]) * ddz;
	}

	return result.y;
}

float Terrain::GetHeightByRaycast(Vector3& position)
{
	UINT x = (UINT)position.x;
	UINT z = (UINT)position.z;

	if (x < 0 || x > width - 2) return FLT_MIN;
	if (z < 0 || z > height - 2) return FLT_MIN;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z + 1) + x;
	index[2] = width * z + (x + 1);
	index[3] = width * (z + 1) + (x + 1);

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].Position;

	Vector3 start = Vector3(position.x, 100.f ,position.z);
	Vector3 direction = Vector3(0, -1, 0);

	float u, v, distance;
	Vector3 result = Vector3(-1, FLT_MIN, -1);

	if (D3DXIntersectTri(&p[0], &p[1], &p[2], &start, &direction, &u, &v, &distance))
		result = p[0] + (p[1] - p[0]) * u + (p[2] - p[0]) * v;

	if (D3DXIntersectTri(&p[3], &p[1], &p[2], &start, &direction, &u, &v, &distance))
		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;

	return result.y;
}

void Terrain::CreateVertexData()
{
	width = heightMap->GetWidth();
	height = heightMap->GetHeight();

	vector<Color> pixels;
	heightMap->ReadPixel(&pixels);

	vertexCount = width * height;
	vertices = new VertexTerrain[vertexCount];

	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index = width * y + x;
			UINT reverse = width * (height - y - 1) + x;

			vertices[index].Position.x = (float)x;
			vertices[index].Position.y = pixels[reverse].r * 255.f / 10.f;
			vertices[index].Position.z = (float)y;

			vertices[index].Uv.x = x / (float)(width - 1);
			vertices[index].Uv.y = 1.f - (y / (float)(height - 1));
		}
	}
	
}

void Terrain::CreateIndexData()
{
	indexCount = (width - 1) * (height - 1) * 6;
	indices = new UINT[indexCount];

	UINT index = 0;
	for (UINT y = 0; y < height - 1; y++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			indices[index + 0] = width * y + x;
			indices[index + 1] = width * (y + 1) + x;
			indices[index + 2] = width * y + (x + 1);

			indices[index + 3] = width * y + (x + 1);
			indices[index + 4] = width * (y + 1) + x;
			indices[index + 5] = width * (y + 1) + (x + 1);

			index += 6;
		}
	}
}

void Terrain::CreateNormalData()
{
	for (UINT i = 0; i < indexCount / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTerrain v0 = vertices[index0];
		VertexTerrain v1 = vertices[index1];
		VertexTerrain v2 = vertices[index2];

		Vector3 e1 = v1.Position - v0.Position;
		Vector3 e2 = v2.Position - v0.Position;

		Vector3 normal;
		D3DXVec3Cross(&normal, &e1, &e2);
		D3DXVec3Normalize(&normal, &normal);

		vertices[index0].Normal += normal;
		vertices[index1].Normal += normal;
		vertices[index2].Normal += normal;
	}

	for (UINT i = 0; i < vertexCount; i++)
		D3DXVec3Normalize(&vertices[i].Normal, &vertices[i].Normal);
}

