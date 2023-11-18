#pragma once
#include "Module.h"
#include <MathGeoLib.h>
#include "SDL.h"


class ModuleCamera :
    public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	//bool CleanUp();
	void SetFOV(float horizontalFOV);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float nearPlane, float farPlane);
	void SetPosition(float x, float y, float z);
	void SetOrientation();
	void LookAt(float3 target_pos);
	void ManageInput();
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();


private:
	float camera_speed = 0.1f;
	Frustum* frustum;
	float4x4* camera_matrix;
};

