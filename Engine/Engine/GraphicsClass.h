#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass/LightShaderClass.h"
#include "LightClass.h"
#include "ShaderClass/TextureShaderClass.h"
#include "BitmapClass.h"
#include "TextClass.h"
#include "FrustumClass.h"
#include "ModelListClass.h"

#include "ImGui/ImGuiEditorClass.h"

#include "DesignPatternHeader.h"

#include "Material.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// 모든 그래픽 관련 기능, 객체를 관리하는 클래스
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int mouseX, int mouseY, float rotationY, bool beginCheck);

private:
	bool Render(float rotation, int mouseX, int mouseY);

	void ModelIntersection(int mouseX, int mouseY);
	bool RaySphereIntersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 raydirection, float radius);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;

	TextClass* m_Text;

	ModelListClass* m_ModelList;
	FrustumClass* m_Frustum;
	ModelClass* m_ModelSphere;

	ImGuiEditorClass* m_ImGui;


	bool m_beginCheck = false;
	int m_screenWidth = 0;
	int m_screenHeight = 0;

	int pickedModelIndex = 0;

	D3DXMATRIX baseViewMatrix;

	GameObject* m_Material;

	ShaderManager* m_ShaderManager;
};

#endif
