#pragma once
#include "Module.h"
#include <MathGeoLib.h>
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
	void SetFOV(float Horizontal_FOV);
	void SetAspectRatio(float Aspect_Ratio);
	void SetPlaneDistances();
	void SetPosition();
	void SetOrientation();
	void LookAt(float x, float y, float z);
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();


private:

	Frustum* frustum;
	float4x4* camera_matrix;
};

