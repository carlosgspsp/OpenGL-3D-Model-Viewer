#include "ModuleCamera.h"
#include "Application.h"
#include "SDL.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

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
	

	int w;
	int h;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);


	frustum->type = FrustumType::PerspectiveFrustum;
	//frustum->pos = float3::zero;
	frustum->pos = float3(0.0f, 4.0f, 8.0f);
	//frustum->front = -float3::unitZ;
	//frustum->up = float3::unitY;
	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 100.0f;
	frustum->verticalFov = math::pi / 4.0;
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f)) * ((float)(w)) / ((float)(h));
	
	//SetFOV(90.0f);
	//SetAspectRatio(16.0f/9.0f);
	LookAt(float3(0.0f, 0.0f, 0.0f));

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
	float aspectRatio = frustum->AspectRatio();
	float aspect1 = tanf(frustum->horizontalFov / 2.0f) / tanf(frustum->verticalFov / 2.0f);
	frustum->horizontalFov = horizontalFOV * (pi/180.0f);

	frustum->verticalFov = 2.0f * atanf(tanf(horizontalFOV * 0.5f)) * (1.0f/aspect1);


	float aspect2 = tanf(frustum->horizontalFov/2.0f) / tanf(frustum->verticalFov/2.0f);
	float aspectRatio2 = frustum->AspectRatio();
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


/*void ModuleCamera::ManageInput(SDL_Event* sdlEvent) {
	float speed = 0.1f;
	
	switch (sdlEvent->key.keysym.sym) {
	case SDLK_w:
		frustum->pos += frustum->front * speed; 
		break;
	case SDLK_s:
		frustum->pos -= frustum->front * speed;
		break;
	case SDLK_a:
		frustum->pos -= frustum->WorldRight() * speed;
		break;
	case SDLK_d:
		frustum->pos += frustum->WorldRight() * speed;
		break;
	}

}*/

void ModuleCamera::ManageInput() {
	

	if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KEY_REPEAT  ) {
		frustum->pos += frustum->front * camera_speed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		frustum->pos -= frustum->front * camera_speed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		frustum->pos -= frustum->WorldRight() * camera_speed;
	}
	if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		frustum->pos += frustum->WorldRight() * camera_speed;
	}

}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{
	ManageInput();
	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{

	return UPDATE_CONTINUE;
}