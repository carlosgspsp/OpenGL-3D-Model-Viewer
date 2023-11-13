#include "ModuleCamera.h"
#include "Application.h"
#include "SDL.h"
#include "ModuleWindow.h"

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
	frustum->front = -float3::unitZ;
	frustum->up = float3::unitY;

	frustum->nearPlaneDistance = 0.1f;
	frustum->farPlaneDistance = 100.0f;
	frustum->verticalFov = math::pi / 4.0;
	frustum->horizontalFov = 2.0f * atanf(tanf(frustum->verticalFov * 0.5f)) * ((float)(w)) / ((float)(h));

	LookAt(0, 0, 0);

	return true;
}


float4x4 ModuleCamera::GetProjectionMatrix() {
	return frustum->ProjectionMatrix();
}

float4x4 ModuleCamera::GetViewMatrix() {
	//*camera_matrix = LookAt(0, 0, 0);
	float4x4 view_matrix = camera_matrix->Inverted();
	return view_matrix;
}

void ModuleCamera::LookAt(float x, float y, float z) {

	//float3 camera_pos(0.0f, 4.0f, 8.0f);
	float3 target_pos(x, y, z);

	

	float3 forward = (target_pos - frustum->pos).Normalized();
	float3 right = Cross(forward, float3::unitY).Normalized();
	float3 up = Cross(right, forward).Normalized();

	*camera_matrix = float4x4(right[0], up[0], -forward[0], frustum->pos[0], right[1], up[1], -forward[1], frustum->pos[1], right[2], up[2], -forward[2], frustum->pos[2], 0, 0, 0, 1);

}

update_status ModuleCamera::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleCamera::PostUpdate()
{

	return UPDATE_CONTINUE;
}