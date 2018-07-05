
#include "stdafx.h"
#include "Camera.h"


Camera::Camera()
{
	_position = XMFLOAT3(0.f, 0.f, 0.f);
	_rotation = XMFLOAT3(0.f, 0.f, 0.f);

	_speed = 10.f;
	_mouseSpeed = 10.f;
}


Camera::Camera(const Camera& other)
{
}


Camera::~Camera()
{
}

//위치 값 셋팅
void Camera::SetPosition(float x, float y, float z)
{
	
	_position.x = x;
	_position.y = y;
	_position.z = z;
	return;
}

//회전 값 셋팅
void Camera::SetRotation(float x, float y, float z)
{
	_rotation.x = x;
	_rotation.y = y;
	_rotation.z = z;
	return;
}

//위치 행렬 반환
XMVECTOR Camera::GetPosition()
{
	return XMLoadFloat3(&_position);
}


//회전 행렬 반환
XMVECTOR Camera::GetRotation()
{
	return XMLoadFloat3(&_rotation);
}

//뷰 행렬 업데이트
XMMATRIX Camera::Update(float duration)
{
	float yaw, pitch, roll;
	XMFLOAT3 up,  lookAt;
	XMMATRIX rotationMatrix;


	up = XMFLOAT3(0.f, 1.f, 0.f);
	lookAt = XMFLOAT3(0.f, 0.f, 1.f);

	pitch = _rotation.x * 0.0174532925f;
	yaw   = _rotation.y * 0.0174532925f;
	roll  = _rotation.z * 0.0174532925f;

	//회전 값으로 카메라 회전 행렬 만듬
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//카메라 up 벡터, LookAtVector 벡터 회전
	XMVECTOR upVec = XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix);
	XMVECTOR lookAtVec = XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix);
	_lookAtVec = lookAtVec;

	//LookAtVector에 이동벡터에 대한 값 적용
	lookAtVec = XMLoadFloat3(&_position) + lookAtVec;

	//뷰 행렬 만들기
	_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&_position), lookAtVec, upVec);

	return _viewMatrix;
}


//광선 방향 계산
Ray Camera::ComputeRay(XMFLOAT2 screenPos, D3D11_VIEWPORT viewPort, XMMATRIX matProj)
{
	//스크린의 위치 비율을 얻기
	float factorX = screenPos.x / viewPort.Width;
	float factorY = (1.0f - (screenPos.y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;


	//[1][][][]
	//[][1][][]
	//[][][1][]
	//[0][0][0][]
	//화각 스케일량을 나눈다


	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, matProj);
	XMVECTOR rayDirection = XMLoadFloat4(&XMFLOAT4(factorX / proj._11, factorY / proj._22, 1.0f, 0.0f));

	//카메라 월드 행렬
	XMMATRIX invView = XMMatrixInverse(nullptr, _viewMatrix);

	//레이의 방향을 카메라 월드기준으로 계산한다
	rayDirection = XMVector4Transform(rayDirection, invView);

	//방향벡터 정규화
	rayDirection = XMVector4Normalize(rayDirection);

	//Ray의 방향
	XMFLOAT3 direction;
	XMStoreFloat3(&direction, rayDirection);

	return Ray{_position, direction};
}



//입력 이벤트
void Camera::DeviceInput(float duration)
{
	MouseInput(duration);
	KeyboardInput(duration);
}


//마우스 이동, 내용 동일
void Camera::MouseInput(float duration)
{
	POINT screenCenter{	WINSIZEX * 0.5f, WINSIZEY * 0.5f };

	if (KEYMANAGER.IsOnceKeyDown(VK_RBUTTON)) {
		MYUTIL::setMousePos(screenCenter.x, screenCenter.y);
	}
	else if(KEYMANAGER.IsStayKeyDown(VK_RBUTTON)) {
		POINT mousePos = MYUTIL::getMousePos();

		float Hori = mousePos.x - screenCenter.x;
		float Vert = mousePos.y - screenCenter.y;
		
		_rotation.x += Vert * duration * _mouseSpeed;
		_rotation.y += Hori * duration * _mouseSpeed;

		MYUTIL::setMousePos(screenCenter.x, screenCenter.y);
	}
}


//키보드를 이용한 카메라 위치 변경
void Camera::KeyboardInput(float duration)
{
	//위치벡터, 카메라 방향벡터, 업 벡터
	XMVECTOR posVec = XMLoadFloat3(&_position);
	XMVECTOR moveVec = XMVector3Normalize(_lookAtVec);
	XMVECTOR upVec = XMVectorSet(0.f, 1.f, 0.f, 0.f);



	if (KEYMANAGER.IsStayKeyDown('W')) {
		posVec += (moveVec * duration * _speed);
	} 
	else if (KEYMANAGER.IsStayKeyDown('S')) {
		posVec -= (moveVec * duration * _speed);
	}

	if (KEYMANAGER.IsStayKeyDown('A')) {
		posVec -= XMVector3Cross(upVec, moveVec) * duration *  _speed;
	}
	else if (KEYMANAGER.IsStayKeyDown('D')) {
		posVec += XMVector3Cross(upVec, moveVec) * duration * _speed;
	}

	if (KEYMANAGER.IsStayKeyDown('Q')) {
		posVec -= (upVec * duration * _speed);
	}
	else if (KEYMANAGER.IsStayKeyDown('E')) {
		posVec += (upVec * duration * _speed);
	}

	//이동에 따른 위치 갱신
	XMStoreFloat3(&_position, posVec);

}
