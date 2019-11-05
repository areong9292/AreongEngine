#include "GraphicsClass.h"
#include "FBXImporter.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_LightShader = nullptr;
	m_Light = nullptr;
	m_TextureShader = nullptr;
	m_Bitmap = nullptr;
	m_Text = nullptr;

	m_ModelList = nullptr;
	m_Frustum = nullptr;
	m_ModelSphere = nullptr;

	m_RenderTexture = nullptr;

	m_ImGui = nullptr;

	m_Material = nullptr;
	m_ShaderManager = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}

// Direct3D ��ü�� �����ϰ� �ʱ�ȭ�Ѵ�
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// ȭ�� �ʺ�� ���� ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct3D ��ü�� �����Ѵ�.
	m_D3D = new D3DClass;
	if (m_D3D == nullptr)
	{
		return false;
	}

	// Direct3D ��ü�� �ʱ�ȭ�Ѵ�.
	// SystemClass���� ���� ������ â ũ�⸸ŭ�� ������
	// DirectX�� �׸� �� �ֵ��� �ϴ� �⺻ ���� ��,
	// Direct3D �ý����� �����Ѵ�
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// ī�޶� ��ü ����
	m_Camera = new CameraClass;
	if (m_Camera == nullptr)
	{
		return false;
	}

	// 2D �׸��� �� ����Ʈ ���
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// ���� ī�޶� �ʱ� ��ġ ����
	m_Camera->SetPosition(0.0f, 0.0f, -100.0f);

	// �ؽ�Ʈ ��ü ����
	m_Text = new TextClass;
	if (m_Text == nullptr)
	{
		return false;
	}

	// �ؽ�Ʈ ��ü �ʱ�ȭ
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// �� ��ü ����
	m_Model = new ModelClass;
	if (m_Model == nullptr)
	{
		return false;
	}

	// �� ��ü �ʱ�ȭ
	// ���� �̸��� ���ڷ� ������ �� �����͸� �д´�
	// ���� �׸��µ� ���Ǵ� �ؽ����� �̸��� ���ڷ� ������
	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/model.txt", L"../Engine/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}


	FBXImporter* fbxImporter;
	fbxImporter = new FBXImporter;
	if (fbxImporter == nullptr)
		return 0;

	result = fbxImporter->LoadFBX(testVector);

	m_Model->test(*testVector);

	// ��ü �� �ε�
	m_ModelSphere = new ModelClass;
	if (m_ModelSphere == nullptr)
	{
		return false;
	}

	result = m_ModelSphere->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/seafloor.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sphere model object.", L"Error", MB_OK);
		return false;
	}
	
	// ���� ���̴� ��ü ����
	m_LightShader = new LightShaderClass;
	if (m_LightShader == nullptr)
	{
		return false;
	}

	// �÷� ���̴� ��ó �ʱ�ȭ
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Light shader object.", L"Error", MB_OK);
		return false;
	}

	// ���� ��ü ����
	m_Light = new LightClass;
	if (m_Light == nullptr)
	{
		return false;
	}

	// �ֺ��� ���� - ����� 15%
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);

	// ���� ���� ���� - ���
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ������ ���� ���� - x��(ȭ���� ���ʿ��� �����ʹ���)
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);

	// ���ݻ籤 �� ����
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	// ���ݻ籤 ���� ���� - ���� �������� �ݻ籤 ������ Ŀ����
	m_Light->SetSpecularPower(32.0f);


	// �� ����Ʈ ��ü �ʱ�ȭ
	m_ModelList = new ModelListClass;
	if (m_ModelList == nullptr)
	{
		return false;
	}
	
	// 25���� ���� ��ġ ��ü ����
	result = m_ModelList->Initialize(m_D3D);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// ����ü ��ü ����
	// Init �� �ʿ� ���� �� �����Ӹ��� ������ִ� �Ŷ�...
	m_Frustum = new FrustumClass;
	if (m_Frustum == nullptr)
	{
		return false;
	}

	// �ؽ��� ���̴� Ŭ������ �����Ѵ�
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// �ؽ��� ���̴��� �ʱ�ȭ�Ѵ�
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// ��Ʈ�� ��ü�� �����Ѵ�
	m_Bitmap = new BitmapClass;
	if (m_Bitmap == nullptr)
	{
		return false;
	}
	
	// ��Ʈ�� ��ü�� �ʱ�ȭ�Ѵ�
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/mouse.dds", 32, 32);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_RenderTexture = new RenderTextureClass;
	if (m_RenderTexture == nullptr)
	{
		return false;
	}

	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	m_ImGui = new ImGuiEditorClass();
	if (m_ImGui == nullptr)
	{
		return false;
	}

	result = m_ImGui->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, m_D3D->GetRenderTargetView(), this, m_screenWidth, m_screenHeight);
	if (!result)
	{
		return false;
	}

	m_ShaderManager = Singleton<ShaderManager>::GetInstance();
	if (m_ShaderManager == nullptr)
	{
		return false;
	}

	result = m_ShaderManager->Initialize(m_D3D->GetDevice(),m_D3D->GetDeviceContext(), hwnd, m_Camera, m_Light);
	if (!result)
	{
		return false;
	}

	return true;
}

// ��� �׷��� ��ü�� ������ �Ͼ��
void GraphicsClass::Shutdown()
{
	// Cleanup
	if (m_Material != nullptr)
	{
		delete m_Material;
		m_Material = nullptr;
	}

	if (m_ImGui != nullptr)
	{
		m_ImGui->Shutdown();
		delete m_ImGui;
		m_ImGui = nullptr;
	}

	if (m_RenderTexture != nullptr)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = nullptr;
	}

	if (m_ModelSphere != nullptr)
	{
		m_ModelSphere->Shutdown();
		delete m_ModelSphere;
		m_ModelSphere = nullptr;
	}

	if (m_Frustum != nullptr)
	{
		delete m_Frustum;
		m_Frustum = nullptr;
	}

	if (m_ModelList != nullptr)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = nullptr;
	}

	if (m_Text != nullptr)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = nullptr;
	}

	if (m_Bitmap != nullptr)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = nullptr;
	}

	if (m_TextureShader != nullptr)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	if (m_Light != nullptr)
	{
		delete m_Light;
		m_Light = nullptr;
	}
	// �ؽ��� ���̴� ��ü ��ȯ
	if (m_LightShader != nullptr)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

	// �� ��ü ��ȯ
	if(m_Model != nullptr)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}
	
	// ī�޶� ��ü ��ȯ
	if(m_Camera != nullptr)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	if (m_D3D != nullptr)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}

	return;
}

bool GraphicsClass::Frame(int mouseX, int mouseY, float rotationY, bool beginCheck)
{
	bool result;

	// ��� ȸ���Ѵ�
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// ī�޶��� ȸ�� ���� �����ؼ� �Է� ���� ����
	// ī�޶� ȸ����Ų��
	m_Camera->SetRotation(0.0f, rotationY, 0.0f);

	// Ŭ�������� ��ŷ ����Ѵ�
	if (beginCheck)
	{
		ModelIntersection(mouseX, mouseY);
	}

	// �� �����Ӹ��� Render�� �ҷ� �׷��� �������� �����Ѵ�
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

ModelListClass * GraphicsClass::GetModelList()
{
	return m_ModelList;
}

// ������ �׸��⸦ ȣ���ϴ� �Լ�
bool GraphicsClass::Render(float rotation)
{
	bool result;

	// ��ü ���� �ؽ��Ŀ� �׸���
	result = RenderToTexture(rotation);
	if(!result)
	{
		return false;
	}

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// ��ü ���� �׸���
	result = RenderScene(rotation);
	if (!result)
	{
		return false;
	}

	/// 2D ����
	// Z���۸� ����
	m_D3D->TurnZBufferOff();
	
	// ���� ���� �Ҵ�
	m_D3D->TurnOnAlphaBlending();
	
	/*result = m_Bitmap->Render(m_D3D->GetDeviceContext(), mouseX, mouseY);
	if (!result)
	{
		return false;
	}

	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, baseViewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}
	
	// ���� �������ϰ� �ִ� ���� ���� ����Ѵ�
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// ���� �׸���
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix2D, orthoMatrix);
	if (!result)
	{
		return false;
	}*/

	// ���� ���� ����
	m_D3D->TurnOffAlphaBlending();

	// Z���۸� �Ҵ�
	m_D3D->TurnZBufferOn();
	/// 2D ��

	// ImGui ���
	m_ImGui->Render(m_RenderTexture->GetShaderResourceView());

	// ���ۿ� �׷��� ���� ȭ�鿡 ǥ���Ѵ�
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture(float rotation)
{
	bool result;
	
	// RTT�� ������ Ÿ���� �ǵ��� �Ѵ�
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// RTT�� Ŭ�����Ѵ�
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
	
	// ���⼭ ���� �׸��� ����� ��� RTT�� �������ȴ�
	result = RenderScene(rotation);
	if(!result)
	{
		return false;
	}
	
	// ������ Ÿ���� RTT���� �ٽ� ����۷� �����Ѵ�
	m_D3D->SetBackBufferRenderTarget();
	
	return true;
}

bool GraphicsClass::RenderScene(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX  scaleMatrix, worldMatrix2D;
	bool result;

	// ��� �� ���� SRT - ������(������, ����, �̵�) ������ ���ؾ��Ѵ�
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX translateMatrix;
	D3DXMATRIX resultMatrix;

	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	bool renderModel;

	// scaleMatrix�� ������ ��ȯ ��� �� ��ü�� �����
	// �̷� ������ ��� ����� ���� ��Ŀ� �����ָ� ������ ��ȯ�� �Ϸ�ȴ�
	D3DXMatrixIdentity(&scaleMatrix);
	D3DXMatrixScaling(&scaleMatrix, 5.0f, 5.0f, 5.0f);

	// ȭ���� ���������� �ʱ�ȭ
	// �� �׸��⸦ �����ϱ� ���� ������ ������ �Ѱ��� ������ ������� �����
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// ���� ī�޶� ��ġ�� �������� �� ����� �����Ѵ�
	m_Camera->Render();

	// ī�޶�, d3d ��ü�� ���� ����, ��, ��������� �����´�
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	rotationMatrix = worldMatrix;
	worldMatrix2D = worldMatrix;
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// ���翵 ����� �����´�
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// ����ü�� ����Ѵ�
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// ���� �� ����Ʈ�� �ִ� ���� ������ �����´�
	modelCount = m_ModelList->GetModelCount();

	// ���� ȭ�鿡�� �׸��� �ִ� ���� ���� �����ϴ� ����
	renderCount = 0;

	// ��� ���� Ȯ���ϸ鼭 �� ���� ����ü �ȿ� ����������
	// �׸���
	for (index = 0; index < modelCount; index++)
	{
		// �ε����� �� ������ �����´�
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		radius = 1.0f;

		// ����ü�ȿ� �������� ���
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);
		if (renderModel)
		{
			if (pickedModelIndex != 0 && index == pickedModelIndex - 1)
			{
				// Y�� �������� ȸ����Ų��
				D3DXMatrixRotationY(&rotationMatrix, rotation);
			}

			// ���� ��ġ�� ���� ��� �̵�
			D3DXMatrixTranslation(&translateMatrix, positionX, positionY, positionZ);

			// ���� ���� ��� ��� - ������ * ȸ�� * �̵� ����� ���ؼ� ���ϴ� ����� ��´�
			resultMatrix = /*scaleMatrix **/ rotationMatrix * translateMatrix;

			if (index != 0)
			{
				// ���� ���ؽ�, �ε��� ���۸� ���������ο� �ִ´�
				m_ModelList->GetModel(index)->Render(m_D3D->GetDeviceContext());

				result = m_ShaderManager->Render(m_ModelList->GetModel(index), resultMatrix, viewMatrix, projectionMatrix);

			}
			else
			{
				result = true;
			}
			if (!result)
			{
				return false;
			}

			if (index == 0)
			{
				// ���� ���ؽ�, �ε��� ���۸� ���������ο� �ִ´�
				m_Model->Render(m_D3D->GetDeviceContext());

				// Y�� �������� ȸ����Ų��
				D3DXMatrixRotationY(&rotationMatrix, rotation);
				// ���� ���� ��� ��� - ������ * ȸ�� * �̵� ����� ���ؼ� ���ϴ� ����� ��´�
				resultMatrix = /*scaleMatrix **/ rotationMatrix * translateMatrix;
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), resultMatrix, viewMatrix, projectionMatrix, m_Model->GetModelMaterial()->GetTexture());

				if (!result)
				{
					return false;
				}
			}

			rotationMatrix = worldMatrix;
			translateMatrix = worldMatrix;

			// �� �׸��鼭 ����� ���� ��Ʈ������ �ٽ� �����Ѵ�
			//m_D3D->GetWorldMatrix(worldMatrix);

			// �� ���� ���� �׷����� �ִ��� Ȯ�ο� ���� ����
			renderCount++;

		}
	}
	m_D3D->GetWorldMatrix(worldMatrix);

	return true;
}

void GraphicsClass::ModelIntersection(int mouseX, int mouseY)
{
	D3DXMATRIX projectionMatrix, viewMatrix, inverseViewMatrix, worldMatrix, translateMatrix, inverseWorldMatrix;
	D3DXVECTOR3 direction, origin, rayOrigin, rayDirection;
	bool intersect, result;

	float pointX, pointY;

	int modelCount = m_ModelList->GetModelCount();

	float positionX, positionY, positionZ;
	D3DXVECTOR4 color;

	pickedModelIndex = 0;

	// ���콺 Ŀ�� ��ǥ�� -1 ~ +1 ������ �̵���Ų��
	pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	// ��������� ����Ͽ� ȭ�� ������ ����
	m_D3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// �� ����� ����
	m_Camera->GetViewMatrix(viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// �� �������� ��ŷ ������ ������ ����Ѵ�
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// ī�޶��� ��ġ �� picking ray�� ������ �����ɴϴ�.
	origin = m_Camera->GetPosition();

	// ������ ��� ������ �����´�
	origin = m_Camera->GetPosition();

	for (int i = 0; i < modelCount; i++)
	{
		m_ModelList->GetData(i, positionX, positionY, positionZ, color);

		// �̵� ��ȯ
		m_D3D->GetWorldMatrix(worldMatrix);
		D3DXMatrixTranslation(&translateMatrix, positionX, positionY, positionZ);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

		// �̵� ��ȯ �Ϸ�� ���� ����� ���� �����´�
		D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

		// �� �������� ���� �������� ��ȯ�Ѵ�
		D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
		D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

		// ���� ���⺤�͸� ����ȭ�Ѵ�
		D3DXVec3Normalize(&rayDirection, &rayDirection);

		// ���� �׽�Ʈ�� ��ŷ ����� ���� �Ǵ��Ѵ�
		intersect = RaySphereIntersect(rayOrigin, rayDirection, 1.0f);

		if (intersect)
		{
			pickedModelIndex = i + 1;
		}
	}

	// ���⼱ �ܼ��� �� ��θ� ���������
	// GetData �޼ҵ�� �� ��ü�� ������ �� �ִ�
	// �װɷ� ���� �����ϰų� �Ϲ� �ɵ�
	if (pickedModelIndex != 0)
		result = m_Text->SetIntersection(true, m_D3D->GetDeviceContext(), m_ModelList->GetModelName(pickedModelIndex - 1));
	else
		result = m_Text->SetIntersection(false, m_D3D->GetDeviceContext());

	return;
}

bool GraphicsClass::RaySphereIntersect(D3DXVECTOR3 rayOrigin, D3DXVECTOR3 rayDirection, float radius)
{
	float a, b, c, discriminant;
	a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (radius * radius);

	discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0.0f)
	{
		return false;
	}

	return true;
}
