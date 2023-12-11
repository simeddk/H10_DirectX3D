#include "Framework.h"
#include "Context.h"
#include "Viewer/Viewport.h"
#include "Viewer/Perspective.h"

Context* Context::instance = NULL;

Context * Context::Get()
{
	//assert(instance != NULL);

	return instance;
}

void Context::Create()
{
	assert(instance == NULL);

	instance = new Context();
}

void Context::Delete()
{
	SafeDelete(instance);
}

Context::Context()
{
	D3DDesc desc = D3D::GetDesc();

	perspective = new Perspective(desc.Width, desc.Height);
	viewport = new Viewport(desc.Width, desc.Height);
	
	camera = new Freedom();
}

Context::~Context()
{
	SafeDelete(perspective);
	SafeDelete(viewport);
	SafeDelete(camera);
}

void Context::ResizeScreen()
{
	perspective->Set(D3D::Width(), D3D::Height());
	viewport->Set(D3D::Width(), D3D::Height());
}

void Context::Update()
{
	camera->Update();
}

void Context::Render()
{
	viewport->RSSetViewport();

	//FPS
	string str = string("FPS : ") + to_string(ImGui::GetIO().Framerate);
	Gui::Get()->RenderText(Vector2(5, 5), Color(1, 0, 0, 1), str);

	//Camera
	Vector3 cameraPosition;
	camera->Position(&cameraPosition);

	Vector3 cameraRotation;
	camera->RotationDegree(&cameraRotation);

	str = "Camera(P) : ";
	str += to_string((int)cameraPosition.x) + ", " + to_string((int)cameraPosition.y) + ", " + to_string((int)cameraPosition.z);
	Gui::Get()->RenderText(Vector2(5, 20), Color(1, 1, 1, 1), str);

	str = "Camera(R) : ";
	str += to_string((int)cameraRotation.x) + ", " + to_string((int)cameraRotation.y) + ", " + to_string((int)cameraRotation.z);
	Gui::Get()->RenderText(Vector2(5, 35), Color(1, 1, 1, 1), str);
}

D3DXMATRIX Context::View()
{
	Matrix view;
	camera->GetMatrix(&view);

	return view;
}

D3DXMATRIX Context::Projection()
{
	D3DXMATRIX projection;
	perspective->GetMatrix(&projection);

	return projection;
}
