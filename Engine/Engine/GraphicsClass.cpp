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

// Direct3D 객체를 생성하고 초기화한다
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// 화면 너비와 높이 저장
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct3D 객체를 생성한다.
	m_D3D = new D3DClass;
	if (m_D3D == nullptr)
	{
		return false;
	}

	// Direct3D 객체를 초기화한다.
	// SystemClass에서 만든 윈도우 창 크기만큼의 공간을
	// DirectX로 그릴 수 있도록 하는 기본 설정 즉,
	// Direct3D 시스템을 설정한다
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// 카메라 객체 생성
	m_Camera = new CameraClass;
	if (m_Camera == nullptr)
	{
		return false;
	}

	// 2D 그리기 용 뷰포트 계산
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// 이후 카메라 초기 위치 설정
	m_Camera->SetPosition(0.0f, 0.0f, -100.0f);

	// 텍스트 객체 생성
	m_Text = new TextClass;
	if (m_Text == nullptr)
	{
		return false;
	}

	// 텍스트 객체 초기화
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// 모델 객체 생성
	m_Model = new ModelClass;
	if (m_Model == nullptr)
	{
		return false;
	}

	// 모델 객체 초기화
	// 모덱의 이름을 인자로 보내서 모델 데이터를 읽는다
	// 모델을 그리는데 사용되는 텍스쳐의 이름을 인자로 보낸다
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

	// 구체 모델 로드
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
	
	// 조명 쉐이더 객체 생성
	m_LightShader = new LightShaderClass;
	if (m_LightShader == nullptr)
	{
		return false;
	}

	// 컬러 쉐이더 객처 초기화
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Light shader object.", L"Error", MB_OK);
		return false;
	}

	// 조명 객체 생성
	m_Light = new LightClass;
	if (m_Light == nullptr)
	{
		return false;
	}

	// 주변광 셋팅 - 흰색의 15%
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);

	// 조명 색상 지정 - 흰색
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 조명의 방향 지정 - x축(화면의 왼쪽에서 오른쪽방향)
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);

	// 정반사광 색 지정
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);

	// 정반사광 강도 지정 - 값이 낮을수록 반사광 범위가 커진다
	m_Light->SetSpecularPower(32.0f);


	// 모델 리스트 객체 초기화
	m_ModelList = new ModelListClass;
	if (m_ModelList == nullptr)
	{
		return false;
	}
	
	// 25개의 랜덤 위치 구체 생성
	result = m_ModelList->Initialize(m_D3D);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// 절두체 객체 생성
	// Init 할 필요 없음 매 프레임마다 계산해주는 거라...
	m_Frustum = new FrustumClass;
	if (m_Frustum == nullptr)
	{
		return false;
	}

	// 텍스쳐 쉐이더 클래스를 생성한다
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// 텍스쳐 쉐이더를 초기화한다
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// 비트맵 객체를 생성한다
	m_Bitmap = new BitmapClass;
	if (m_Bitmap == nullptr)
	{
		return false;
	}
	
	// 비트맵 객체를 초기화한다
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

// 모든 그래픽 객체의 해제가 일어난다
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
	// 텍스쳐 쉐이더 객체 반환
	if (m_LightShader != nullptr)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

	// 모델 객체 반환
	if(m_Model != nullptr)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
	}
	
	// 카메라 객체 반환
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

	// 계속 회전한다
	static float rotation = 0.0f;

	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);

	// 카메라의 회전 값을 셋팅해서 입력 값에 따라
	// 카메라를 회전시킨다
	m_Camera->SetRotation(0.0f, rotationY, 0.0f);

	// 클릭했으면 픽킹 계산한다
	if (beginCheck)
	{
		ModelIntersection(mouseX, mouseY);
	}

	// 매 프레임마다 Render를 불러 그래픽 렌더링을 수행한다
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

// 실제로 그리기를 호출하는 함수
bool GraphicsClass::Render(float rotation)
{
	bool result;

	// 전체 씬을 텍스쳐에 그린다
	result = RenderToTexture(rotation);
	if(!result)
	{
		return false;
	}

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 전체 씬을 그린다
	result = RenderScene(rotation);
	if (!result)
	{
		return false;
	}

	/// 2D 시작
	// Z버퍼를 끈다
	m_D3D->TurnZBufferOff();
	
	// 알파 블렌딩 켠다
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
	
	// 현재 렌더링하고 있는 모델의 수를 출력한다
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// 문장 그린다
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix2D, orthoMatrix);
	if (!result)
	{
		return false;
	}*/

	// 알파 블렌딩 끈다
	m_D3D->TurnOffAlphaBlending();

	// Z버퍼를 켠다
	m_D3D->TurnZBufferOn();
	/// 2D 끝

	// ImGui 출력
	m_ImGui->Render(m_RenderTexture->GetShaderResourceView());

	// 버퍼에 그려진 씬을 화면에 표시한다
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture(float rotation)
{
	bool result;
	
	// RTT가 렌더링 타겟이 되도록 한다
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// RTT를 클리어한다
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 0.0f, 1.0f);
	
	// 여기서 씬을 그리면 백버퍼 대신 RTT에 렌더링된다
	result = RenderScene(rotation);
	if(!result)
	{
		return false;
	}
	
	// 렌더링 타겟을 RTT에서 다시 백버퍼로 변경한다
	m_D3D->SetBackBufferRenderTarget();
	
	return true;
}

bool GraphicsClass::RenderScene(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX  scaleMatrix, worldMatrix2D;
	bool result;

	// 행렬 총 조합 SRT - 스자이(스케일, 자전, 이동) 순으로 곱해야한다
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX translateMatrix;
	D3DXMATRIX resultMatrix;

	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	D3DXVECTOR4 color;
	bool renderModel;

	// scaleMatrix을 스케일 변환 행렬 그 자체로 만든다
	// 이런 식으로 행렬 만들고 월드 행렬에 곱해주면 스케일 변환이 완료된다
	D3DXMatrixIdentity(&scaleMatrix);
	D3DXMatrixScaling(&scaleMatrix, 5.0f, 5.0f, 5.0f);

	// 화면을 검은색으로 초기화
	// 씬 그리기를 시작하기 위해 버퍼의 내용을 한가지 색으로 덮어씌워서 지운다
	m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// 현재 카메라 위치를 기준으로 뷰 행렬을 생성한다
	m_Camera->Render();

	// 카메라, d3d 객체로 부터 월드, 뷰, 투영행렬을 가져온다
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	rotationMatrix = worldMatrix;
	worldMatrix2D = worldMatrix;
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// 정사영 행렬을 가져온다
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 절두체를 계산한다
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// 현재 모델 리스트에 있는 모델의 갯수를 가져온다
	modelCount = m_ModelList->GetModelCount();

	// 현재 화면에서 그리고 있는 모델의 수를 저장하는 변수
	renderCount = 0;

	// 모든 모델을 확인하면서 그 모델이 절두체 안에 속해있으면
	// 그린다
	for (index = 0; index < modelCount; index++)
	{
		// 인덱스로 모델 정보를 가져온다
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		radius = 1.0f;

		// 절두체안에 속해있을 경우
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);
		if (renderModel)
		{
			if (pickedModelIndex != 0 && index == pickedModelIndex - 1)
			{
				// Y축 기준으로 회전시킨다
				D3DXMatrixRotationY(&rotationMatrix, rotation);
			}

			// 모델의 위치로 월드 행렬 이동
			D3DXMatrixTranslation(&translateMatrix, positionX, positionY, positionZ);

			// 최종 월드 행렬 계산 - 스케일 * 회전 * 이동 행렬을 곱해서 원하는 결과를 얻는다
			resultMatrix = /*scaleMatrix **/ rotationMatrix * translateMatrix;

			if (index != 0)
			{
				// 모델의 버텍스, 인덱스 버퍼를 파이프라인에 넣는다
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
				// 모델의 버텍스, 인덱스 버퍼를 파이프라인에 넣는다
				m_Model->Render(m_D3D->GetDeviceContext());

				// Y축 기준으로 회전시킨다
				D3DXMatrixRotationY(&rotationMatrix, rotation);
				// 최종 월드 행렬 계산 - 스케일 * 회전 * 이동 행렬을 곱해서 원하는 결과를 얻는다
				resultMatrix = /*scaleMatrix **/ rotationMatrix * translateMatrix;
				result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), resultMatrix, viewMatrix, projectionMatrix, m_Model->GetModelMaterial()->GetTexture());

				if (!result)
				{
					return false;
				}
			}

			rotationMatrix = worldMatrix;
			translateMatrix = worldMatrix;

			// 모델 그리면서 변경된 월드 매트릭스를 다시 리셋한다
			//m_D3D->GetWorldMatrix(worldMatrix);

			// 몇 개의 모델이 그려지고 있는지 확인용 변수 증가
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

	// 마우스 커서 좌표를 -1 ~ +1 범위로 이동시킨다
	pointX = ((2.0f * (float)mouseX) / (float)m_screenWidth) - 1.0f;
	pointY = (((2.0f * (float)mouseY) / (float)m_screenHeight) - 1.0f) * -1.0f;

	// 투영행렬을 사용하여 화면 비율에 맞춤
	m_D3D->GetProjectionMatrix(projectionMatrix);
	pointX = pointX / projectionMatrix._11;
	pointY = pointY / projectionMatrix._22;

	// 역 뷰행렬 구함
	m_Camera->GetViewMatrix(viewMatrix);
	D3DXMatrixInverse(&inverseViewMatrix, NULL, &viewMatrix);

	// 뷰 공간에서 피킹 레이의 방향을 계산한다
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;

	// 카메라의 위치 인 picking ray의 원점을 가져옵니다.
	origin = m_Camera->GetPosition();

	// 레이의 출발 지점을 가져온다
	origin = m_Camera->GetPosition();

	for (int i = 0; i < modelCount; i++)
	{
		m_ModelList->GetData(i, positionX, positionY, positionZ, color);

		// 이동 변환
		m_D3D->GetWorldMatrix(worldMatrix);
		D3DXMatrixTranslation(&translateMatrix, positionX, positionY, positionZ);
		D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);

		// 이동 변환 완료된 월드 행렬의 역을 가져온다
		D3DXMatrixInverse(&inverseWorldMatrix, NULL, &worldMatrix);

		// 뷰 공간에서 월드 공간으로 전환한다
		D3DXVec3TransformCoord(&rayOrigin, &origin, &inverseWorldMatrix);
		D3DXVec3TransformNormal(&rayDirection, &direction, &inverseWorldMatrix);

		// 레이 방향벡터를 정규화한다
		D3DXVec3Normalize(&rayDirection, &rayDirection);

		// 교차 테스트로 피킹 됬는지 여부 판단한다
		intersect = RaySphereIntersect(rayOrigin, rayDirection, 1.0f);

		if (intersect)
		{
			pickedModelIndex = i + 1;
		}
	}

	// 여기선 단순히 모델 경로를 출력하지만
	// GetData 메소드로 모델 자체를 가져올 수 있다
	// 그걸로 모델을 조작하거나 하믄 될듯
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
