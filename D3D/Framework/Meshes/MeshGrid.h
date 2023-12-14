#pragma once

class MeshGrid : public Mesh
{
public:
	MeshGrid(Shader* shader, float tileU = 1.f, float tileV = 1.f);
	~MeshGrid();

private:
	void Create() override;

private:
	float tileU, tileV;
};