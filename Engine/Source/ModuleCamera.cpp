#include "ModuleCamera.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "SDL.h"


ModuleCamera::ModuleCamera()
{

}

// Destructor
ModuleCamera::~ModuleCamera()
{
}


// Called before render is available
bool ModuleCamera::Init()
{
	frustum = new Frustum();
	camera_matrix = new float4x4();
	

	//int w;
	//int h;
	//SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
	float2 screenSize = App->GetWindow()->GetScreenSize();

	frustum->type = FrustumType::PerspectiveFrustum;
	//frustum->pos = float3::zero;
	frustum->pos = float3(0.0f, 4.0f, 8.0f);
	//frustum->front = -float3::unitZ;
	//frustum->up = float3::unitY;
	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 1000.0f;
	frustum->verticalFov = math::pi / 4.0f;
	//frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f)) * ((float)(w)) / ((float)(h));
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f) * screenSize.x / screenSize.y);
	float angle = RadToDeg(frustum->horizontalFov);

	//SetFOV(61.39f);
	//SetAspectRatio(16.0f/9.0f);
	LookAt(float3(0.0f, 0.0f, 0.0f));

	angle = RadToDeg(frustum->verticalFov);

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	mouseSensitivity = float2(0.1f, 0.1f);
	panSensitivity = float2(0.005f, 0.005f);
	zoomSensitivity = 0.1f;
	yaw = -90.0f;
	pitch = -RadToDeg(float3(0.0, 0.0, 1.0).AngleBetween(frustum->pos));
	//pitch = 0;

	verticalPan = 0.0f;
	horizontalPan = 0.0f;

	return true;
}


float4x4 ModuleCamera::GetProjectionMatrix() {
	return frustum->ProjectionMatrix();
}

float4x4 ModuleCamera::GetViewMatrix() {
	//*camera_matrix = LookAt(0, 0, 0);
	//LookAt(frustum->front);

	//float4x4 view_matrix = camera_matrix->Inverted();
	float4x4 view_matrix = frustum->ViewMatrix();
	return view_matrix;
}

void ModuleCamera::LookAt(float3 target_pos) {

	float3 forward = (target_pos - frustum->pos).Normalized();
	float3 right = Cross(forward, float3::unitY).Normalized();
	float3 up = Cross(right, forward).Normalized();

	frustum->front = forward;
	frustum->up = up;

	*camera_matrix = float4x4(right[0], up[0], -forward[0], frustum->pos[0], right[1], up[1], -forward[1], frustum->pos[1], right[2], up[2], -forward[2], frustum->pos[2], 0, 0, 0, 1);

}


void ModuleCamera::SetFOV(float horizontalFOV) { //DEBUGEAR
	//int w;
	//int h;
	//SDL_GetWindowSize(App->GetWindow()->window, &w, &h);

	float2 screenSize = App->GetWindow()->GetScreenSize();

	//float aspectRatio = frustum->AspectRatio();
	//float aspect1 = tanf(frustum->horizontalFov / 2.0f) / tanf(frustum->verticalFov / 2.0f);
	float aspect_normal = screenSize.x / screenSize.y;
	frustum->horizontalFov = DegToRad(horizontalFOV);

	frustum->verticalFov = 2.0f * atanf(tanf(frustum->horizontalFov * 0.5f) * (1.0f / aspect_normal));


	//float aspect2 = tanf(frustum->horizontalFov / 2.0f) / tanf(frustum->verticalFov / 2.0f);
	//float aspectRatio2 = frustum->AspectRatio();
	//float aspect_normal2 = (float)w / (float)h;
}

void ModuleCamera::SetPlaneDistances(float nearPlane, float farPlane) {
	frustum->nearPlaneDistance = nearPlane;
	frustum->farPlaneDistance = farPlane;
}

void ModuleCamera::SetAspectRatio(float aspectRatio) {
	frustum->verticalFov = 2.0f * atanf(tanf(frustum->horizontalFov * 0.5f)) * (1.0f / aspectRatio);
}

void ModuleCamera::SetPosition(float x, float y, float z) {
	frustum->pos = float3(x, y, z);
}

void ModuleCamera::ManageKeyboardInput() {
	if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		cameraSpeed *= 2.0f;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		frustum->pos += frustum->front * cameraSpeed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		frustum->pos -= frustum->front * cameraSpeed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		frustum->pos -= frustum->WorldRight() * cameraSpeed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		frustum->pos += frustum->WorldRight() * cameraSpeed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) {
		frustum->pos += float3::unitY * cameraSpeed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) {
		frustum->pos -= float3::unitY * cameraSpeed;
	}

}

void ModuleCamera::ManageMouseInput() {
	if (App->GetInput()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && App->GetInput()->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
	CameraOrbit();
	}
	else if (App->GetInput()->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		CameraRotation();
	}
	else if (App->GetInput()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
		CameraPan();
	}
	else if (App->GetInput()->GetMouseWheel().y != 0) {
		CameraZoom();
	}
	
}
void ModuleCamera::CameraRotation() {


	float2 mousePosition = App->GetInput()->GetMousePosition();
	float2 lastMousePosition = App->GetInput()->GetLastMousePosition();
	float2 offset(lastMousePosition - mousePosition);

	offset.x *= mouseSensitivity.x;
	offset.y *= mouseSensitivity.y;

	yaw -= offset.x;
	pitch += offset.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	float3 newfront;
	newfront.x = cosf(DegToRad(yaw)) * cosf(DegToRad(pitch));
	newfront.y = sinf(DegToRad(pitch));
	newfront.z = sinf(DegToRad(yaw)) * cosf(DegToRad(pitch));
	newfront.Normalize();

	frustum->front = newfront;
	float3 right = frustum->front.Cross(float3::unitY).Normalized();
	frustum->up = right.Cross(frustum->front).Normalized();

	/*float3x3 rotationDeltaMatrix(
		float3(cosf(yaw)*cosf(pitch), -sinf(yaw), cosf(yaw)*sinf(pitch)),
		float3(sinf(yaw)*cosf(pitch), cos(yaw), sinf(yaw)*sinf(pitch)),
		float3(-sinf(pitch), 0.0f, cosf(pitch))
	);


	float3 oldFront = frustum->front.Normalized();
	float3 oldUp = frustum->up.Normalized();

	frustum->front = rotationDeltaMatrix.MulDir(oldFront);
	frustum->up = rotationDeltaMatrix.MulDir(oldUp);*/




}
void ModuleCamera::CameraPan() {
	float2 mousePosition = App->GetInput()->GetMousePosition();
	float2 lastMousePosition = App->GetInput()->GetLastMousePosition();
	float2 offset(lastMousePosition - mousePosition);

	offset.x *= panSensitivity.x;
	offset.y *= panSensitivity.y;

	 horizontalPan -= offset.x;
	 verticalPan += offset.y;

	 frustum->pos += float3::unitY *offset.y;
	 frustum->pos += frustum->WorldRight() * offset.x;

}


void ModuleCamera::CameraZoom() {
	float2 mouseWheel = App->GetInput()->GetMouseWheel();
	frustum->pos += frustum->front * zoomSensitivity * mouseWheel.y;
}



void ModuleCamera::CameraOrbit() {

	float radius = frustum->pos.Distance(float3::zero);
	float2 mousePosition = App->GetInput()->GetMousePosition();
	float2 lastMousePosition = App->GetInput()->GetLastMousePosition();
	float2 offset(lastMousePosition - mousePosition);

	offset.x *= mouseSensitivity.x;
	offset.y *= mouseSensitivity.y;

	yaw -= offset.x;
	pitch += offset.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	frustum->pos = float3::zero;

	float3 newfront;
	newfront.x = cosf(DegToRad(yaw)) * cosf(DegToRad(pitch));
	newfront.y = sinf(DegToRad(pitch));
	newfront.z = sinf(DegToRad(yaw)) * cosf(DegToRad(pitch));
	newfront.Normalize();

	frustum->front = newfront;
	float3 right = frustum->front.Cross(float3::unitY).Normalized();
	frustum->up = right.Cross(frustum->front).Normalized();
	 

	frustum->Translate(-frustum->front * radius);


}
update_status ModuleCamera::PreUpdate()
{
	float currentFrame = SDL_GetTicks();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	cameraSpeed = 0.1f * deltaTime / 10;
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
	ManageKeyboardInput();
	ManageMouseInput();
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{

	return UPDATE_CONTINUE;
}