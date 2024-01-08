#include "Framework.h"
#include "ModelMesh.h"

//-----------------------------------------------------------------------------
// @ ModelMesh
// 1. Bone Index -> CBuffer Ready
// 2. Bining VertexBuffer, IndexBuffer
// 3. Render to MeshPart
//-----------------------------------------------------------------------------
ModelMesh::ModelMesh()
{
	boneBuffer = new ConstantBuffer(&boneDesc, sizeof(BoneDesc));
}

ModelMesh::~ModelMesh()
{
	SafeDelete(transform);
	SafeDelete(perFrame);

	SafeDeleteArray(vertices);
	SafeDeleteArray(indices);

	SafeDelete(vertexBuffer);
	SafeDelete(indexBuffer);

	SafeDelete(boneBuffer);

	for (ModelMeshPart* part : meshParts)
		SafeDelete(part);
}

void ModelMesh::Binding(Model* model)
{
	vertexBuffer = new VertexBuffer(vertices, vertexCount, sizeof(Model::VertexModel));
	indexBuffer = new IndexBuffer(indices, indexCount);

	for (ModelMeshPart* part : meshParts)
		part->Binding(this);
}

void ModelMesh::Pass(UINT val)
{
	for (ModelMeshPart* part : meshParts)
		part->Pass(val);
}

void ModelMesh::SetShader(Shader* shader)
{
	this->shader = shader;

	SafeDelete(transform);
	transform = new Transform(shader);

	SafeDelete(perFrame);
	perFrame = new PerFrame(shader);

	sBoneBuffer = shader->AsConstantBuffer("CB_Bones");

	for (ModelMeshPart* part : meshParts)
		part->SetShader(shader);
}

void ModelMesh::Update()
{
	//¡Ú¡Ú¡Ú cBuffer Value = bone Index from File(*.mesh)
	boneDesc.BoneIndex = boneIndex;

	perFrame->Update();
	transform->Update();

	for (ModelMeshPart* part : meshParts)
		part->Update();
}

void ModelMesh::Render()
{
	boneBuffer->Render();
	sBoneBuffer->SetConstantBuffer(boneBuffer->Buffer());

	perFrame->Render();
	transform->Render();

	vertexBuffer->Render();
	indexBuffer->Render();

	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (ModelMeshPart* part : meshParts)
		part->Render();
}

void ModelMesh::Transforms(Matrix* transforms)
{
	memcpy(boneDesc.Trasnforms, transforms, sizeof(Matrix) * MAX_MODEL_BONES);
}

void ModelMesh::SetTransform(Transform* tranform)
{
	this->transform->Set(tranform);
}

//-----------------------------------------------------------------------------
// @ ModelMeshPart
// 1. Draw Primitive Call
// 2. Bind Material
//-----------------------------------------------------------------------------
ModelMeshPart::ModelMeshPart()
{
}

ModelMeshPart::~ModelMeshPart()
{
}

void ModelMeshPart::Update()
{
}

void ModelMeshPart::Render()
{
	shader->DrawIndexed(0, pass, indexCount, startIndex);
}

void ModelMeshPart::Biding(Model* model)
{
}

void ModelMeshPart::SetShader(Shader* shader)
{
	this->shader = shader;
}
