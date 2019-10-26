#include "TextureShaderClass.h"

TextureShaderClass::TextureShaderClass()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
	m_sampleState = nullptr;
}

TextureShaderClass::TextureShaderClass(const TextureShaderClass& other)
{
}


TextureShaderClass::~TextureShaderClass()
{
}

bool TextureShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// 쉐이더를 초기화하는 메소드
	// HLSL 쉐이더 파일의 이름을 넘겨준다
	result = InitializeShader(device, hwnd, L"../Engine/HLSL/texture.vs", L"../Engine/HLSL/texture.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void TextureShaderClass::Shutdown()
{
	// 버텍스 쉐이더, 픽셀 쉐이더 관련들을 반환한다
	ShutdownShader();

	return;
}

bool TextureShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	bool result;

	// 렌더링에 사용할 쉐이더의 인자를 입력한다
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture);
	if (!result)
	{
		return false;
	}

	// 쉐이더로 준비된 버퍼를 그린다
	RenderShader(deviceContext, indexCount);

	return true;
}

// 쉐이더 파일을 불러오고 DirectX와 GPU에서 사용 가능하도록 한다
bool TextureShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// 이 쉐이더에서 입력 요소를 2개를 사용한다
	// 입력 요소 벡터에 대한 2개의 입력 레이아웃 생성이 필요하다
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	// 텍스쳐 샘플러의 desc
	D3D11_SAMPLER_DESC samplerDesc;

	// 이 함수에서 사용하는 포인터들을 null로 설정
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// 쉐이더 프로그램을 버퍼로 컴파일한다
	// 정점 셰이더를 컴파일한다
	result = D3DX11CompileFromFile(
		vsFilename,						// 쉐이더 파일 이름
		NULL,
		NULL,
		"TextureVertexShader",			// 쉐이더 이름
		"vs_5_0",						// 쉐이더 버전
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexShaderBuffer,			// 쉐이더가 컴파일 될 버퍼
		&errorMessage,					// 에러 문자열
		NULL);
	if (FAILED(result))
	{
		// 셰이더가 컴파일에 실패하면 에러 메세지를 기록
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// 에러 메세지가 없다면 셰이더 파일을 찾지 못한 것
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// 픽셀 셰이더를 컴파일한다
	result = D3DX11CompileFromFile(
		psFilename,						// 쉐이더 파일 이름
		NULL,
		NULL,
		"TexturePixelShader",			// 쉐이더 이름
		"ps_5_0",						// 쉐이더 버전
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&pixelShaderBuffer,				// 쉐이더가 컴파일 될 버퍼
		&errorMessage,					// 에러 문자열
		NULL);
	if (FAILED(result))
	{
		// 셰이더 컴파일이 실패하면 에러 메세지를 기록합니다.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// 에러 메세지가 없다면 단순히 셰이더 파일을 찾지 못한 것입니다.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// 버텍스, 픽셀 쉐이더가 잘 컴파일되면 쉐이더 버퍼가 생성되는데
	// 이를 이용하여 쉐이더 객체를 생성한다
	// 버퍼로부터 버텍스 쉐이더 객체 생성
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// 버퍼로부터 픽셀 쉐이더를 생성
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// 쉐이더에서 사용할 버텍스 데이터의 레이아웃을 생성한다
	// 쉐이더에서 위치 벡터와 색상 벡터를 사용한다
	// 레이아웃에 각각의 벡터 크기를 포함하는 두 레이아웃을 만든다

	// 정점 입력 레이아웃 description을 작성
	// 이 설정은 ModelClass와 셰이더에 있는 VertexType와 일치해야 한다

	// SemanticName - 이 요소가 레이아웃에서 어떻게 사용되는지 알려준다
	// 버텍스 쉐이더에서 입력은 POSITION, TEXCOORD이므로 2개 이름을 지정한다
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;

	// 위치값에는 x,y,z만 사용
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;

	// 버퍼에 데이터가 어떻게 배열되는가
	// 0부터 시작함
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;



	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;

	// 텍스쳐 값에는 u,v 2가지 요소 사용
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;

	// 위치벡터 사용하고 다음 요소 시작지점을 자동으로 찾는다
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// 레이아웃 desc작성 완료 후 Direct3D 장치로 입력 레이아웃을 생성한다
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// 버텍스 입력 레이아웃을 생성
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// 더 이상 사용되지 않는 정점 셰이더 퍼버와 픽셀 셰이더 버퍼를 해제
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// 쉐이더 사용을 위한 마지막 단계는 상수 버퍼이다
	// 쉐이더의 전역변수인 월드, 뷰, 투영행렬

	// 정점 셰이더에 있는 행렬 상수 버퍼의 description을 작성
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);

	// 상수 버퍼에 이 버퍼를 사용한다는 것을 설정한다
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// 상수 버퍼 포인터를 만들어 이 클래스에서
	// 버텍스 셰이더 상수 버퍼에 접근할 수 있게 한다
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// SamplerState 생성
	// Filter - 최종 도형 표면에서 텍스쳐의 어느 픽셀이 사용, 혼합될 것인가
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	// 0.0f, 1.0f 사이에 값이 있도록 wrap 설정
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	// SamplerState 생성
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureShaderClass::ShutdownShader()
{
	// 샘플러 상태를 해제한다
	if (m_sampleState != NULL)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}

	// 상수 버퍼를 해제한다
	if (m_matrixBuffer != nullptr)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	// 레이아웃을 해제한다
	if (m_layout != nullptr)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// 픽셀 셰이더를 해제한다
	if (m_pixelShader != nullptr)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// 정점 셰이더를 해제한다
	if (m_vertexShader != nullptr)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	return;
}

void TextureShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// 에러 메세지를 담고 있는 문자열 버퍼의 포인터를 가져옵니다.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// 메세지의 길이를 가져옵니다.
	bufferSize = errorMessage->GetBufferSize();

	// 파일을 열고 안에 메세지를 기록합니다.
	fout.open("shader-error.txt");

	// 에러 메세지를 씁니다.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// 파일을 닫습니다.
	fout.close();

	// 에러 메세지를 반환합니다.
	errorMessage->Release();
	errorMessage = nullptr;

	// 컴파일 에러가 있음을 팝업 메세지로 알려줍니다.
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// 쉐이더의 전역 변수를 쉽게 다룰 수 있게 한다
// GraphicsClass에서 만들어진 행렬들을 사용한다
bool TextureShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	// transpose - 전치, 행과 열을 교환한다
	// 행렬을 transpose하여 쉐이더에서 사용할 수 있게 한다
	// 쉐이더에서나 directX -> openGL로 넘어갈 경우에도 전치를 사용하는 듯 - 서로 좌표계가 다르다

	// 쉐이더에서 전치행렬을 사용하는 이유
	// 쉐이더의 변환 매크로, 변환 명령들이 열 주도 행렬들에 적합하기 때문
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// 상수 버퍼의 내용을 쓸 수 있도록 잠근다
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터를 가져온다
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// 상수 버퍼에 행렬을 복사한다
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 푼다
	deviceContext->Unmap(m_matrixBuffer, 0);

	// 버텍스 쉐이더에서의 상수 버퍼의 위치를 설정한다
	bufferNumber = 0;

	// 버텍스 쉐이더의 상수 버퍼를 바뀐 값으로 바꾼다
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// 픽셀 쉐이더에 텍스쳐를 설정한다
	// 텍스쳐는 반드시 버퍼에 렌더링이 일어나기 전에 설정되어 있어야한다
	deviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}

// SetShaderParameters보다 먼저 호출되어 쉐이더의 인자들을 올바르게 셋팅한다
// 1. 입력 레이아웃을 Input assembler에 연결한다
// 이 연결로 GPU 버텍스 버퍼의 자료구조를 알게 된다
// 2. 버텍스 버퍼를 그리기 위한 버텍스, 픽셀 쉐이더를 설정한다
// 3. 픽셀 쉐이더의 샘플러 상태를 설정한다
// 쉐이더가 설정되면 deviceContext에서 DrawIndexed 메소드로 삼각형을 그린다
void TextureShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 1. 버텍스 입력 레이아웃을 설정한다
	deviceContext->IASetInputLayout(m_layout);

	// 2. 삼각형을 그릴 버텍스, 픽셀 쉐이더를 설정한다
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 3. 픽셀 쉐이더의 샘플러 상태를 설정한다
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// 삼각형을 그린다
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
