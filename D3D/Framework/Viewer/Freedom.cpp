#include "Framework.h"
#include "Freedom.h"

Freedom::Freedom()
{
}

Freedom::~Freedom()
{
}

void Freedom::Update()
{
	if (Mouse::Get()->Press(1) == false) return;

	//Move
	{
		Vector3 position;
		Position(&position);

		Vector3 right = Right();
		Vector3 up = Up();
		Vector3 forward = Forward();

		if (Keyboard::Get()->Press('W'))
			position += forward * moveSpeed * Time::Delta();
		else if (Keyboard::Get()->Press('S'))
			position -= forward * moveSpeed * Time::Delta();

		if (Keyboard::Get()->Press('D'))
			position += right * moveSpeed * Time::Delta();
		else if (Keyboard::Get()->Press('A'))
			position -= right * moveSpeed * Time::Delta();

		if (Keyboard::Get()->Press('E'))
			position += up * moveSpeed * Time::Delta();
		else if (Keyboard::Get()->Press('Q'))
			position -= up * moveSpeed * Time::Delta();

		Position(position);
	}

	//Rotation
	{
		Vector3 rotation;
		Rotation(&rotation);

		Vector3 mouseDelta = Mouse::Get()->GetMoveValue();
		rotation.x += mouseDelta.y * rotationSpeed * Time::Delta();
		rotation.y += mouseDelta.x * rotationSpeed * Time::Delta();
		rotation.z = 0.f;

		Rotation(rotation);
	}
}

void Freedom::Speed(float moveSpeed, float rotationSpeed)
{
	this->moveSpeed = moveSpeed;
	this->rotationSpeed = rotationSpeed;
}
