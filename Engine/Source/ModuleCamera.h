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
	bool CleanUp();
	void SetFOV(float horizontalFOV);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float nearPlane, float farPlane);
	void SetPosition(float x, float y, float z);
	void SetOrientation();
	void LookAt(float3 target_pos);
	void ManageKeyboardInput();
	void ManageMouseInput();
	void CameraRotation();
	void CameraPan();
	void CameraZoom();
	void CameraOrbit();
	void FocusGeometry(float3 maxPos, float3 minPos);
	const float3* GetPosition() const { return &frustum->pos; };
	float4x4 GetProjectionMatrix();
	float4x4 GetViewMatrix();


private:
	float cameraSpeed;
	float deltaTime, lastFrame;
	float yaw, pitch;
	float verticalPan, horizontalPan;
	float2 mouseSensitivity, panSensitivity;
	float zoomSensitivity;
	Frustum* frustum;
	float4x4 camera_matrix;
};

