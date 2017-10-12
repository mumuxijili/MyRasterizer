#pragma once
#include "Math3D.h"
#include "Util.h"

class Camera
{
public:
	Camera() {};
	~Camera() {};
	Camera(Vec4 pos, Vec4 dir, float fov, float near, float far);
	Vec4 m_Pos; // camera position(world space)


	//      |v
	//      |
	//      |
	//  cam /------->target
	//     /         n
	//    /u
	Vec4 m_Dir; // look at direction
	Vec4 m_u;
	Vec4 m_v;
	Vec4 m_n;
	Vec4 m_Target; // target position(world space)

	float view_dist_h;
	float view_dist_v; // 水平视距和垂直视距，在透视变换中使用
	
	float m_Fov;
	float m_Near;
	float m_Far;
	float m_Aspect; // width/height

	float m_ViewportWidth; // screen/viewport size
	float m_ViewportHeight;
};