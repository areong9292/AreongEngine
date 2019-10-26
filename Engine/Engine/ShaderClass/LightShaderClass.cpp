#include "LightShaderClass.h"

LightShaderClass::LightShaderClass()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
	m_sampleState = nullptr;
	m_cameraBuffer = nullptr;
	m_lightBuffer = nullptr;
}

LightShaderClass::LightShaderClass(const LightShaderClass& other)
{
}


LightShaderClass::~LightShaderClass()
{
}

bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// ���̴��� �ʱ�ȭ�ϴ� �޼ҵ�
	// HLSL ���̴� ������ �̸��� �Ѱ��ش�
	result = InitializeShader(device, hwnd, L"../Engine/HLSL/Light.vs", L"../Engine/HLSL/Light.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void LightShaderClass::Shutdown()
{
	// ���ؽ� ���̴�, �ȼ� ���̴� ���õ��� ��ȯ�Ѵ�
	ShutdownShader();

	return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	bool result;

	// �������� ����� ���̴��� ���ڸ� �Է��Ѵ�
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor, cameraPosition, specularColor, specularPower);
	if (!result)
	{
		return false;
	}

	// ���̴��� �غ�� ���۸� �׸���
	RenderShader(deviceContext, indexCount);

	return true;
}

// ���̴� ������ �ҷ����� DirectX�� GPU���� ��� �����ϵ��� �Ѵ�
bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// �� ���̴����� �Է� ��Ҹ� 3���� ����Ѵ�
	// �Է� ��� ���Ϳ� ���� 3���� �Է� ���̾ƿ� ������ �ʿ��ϴ�
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

	unsigned int numElements;

	// �ؽ��� ���÷��� desc
	D3D11_SAMPLER_DESC samplerDesc;

	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC cameraBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;

	// �� �Լ����� ����ϴ� �����͵��� null�� ����
	errorMessage = nullptr;
	vertexShaderBuffer = nullptr;
	pixelShaderBuffer = nullptr;

	// ���̴� ���α׷��� ���۷� �������Ѵ�
	// ���� ���̴��� �������Ѵ�
	result = D3DX11CompileFromFile(
		vsFilename,						// ���̴� ���� �̸�
		NULL,
		NULL,
		"LightVertexShader",			// ���̴� �̸�
		"vs_5_0",						// ���̴� ����
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&vertexShaderBuffer,			// ���̴��� ������ �� ����
		&errorMessage,					// ���� ���ڿ�
		NULL);
	if (FAILED(result))
	{
		// ���̴��� �����Ͽ� �����ϸ� ���� �޼����� ���
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
		}
		// ���� �޼����� ���ٸ� ���̴� ������ ã�� ���� ��
		else
		{
			MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// �ȼ� ���̴��� �������Ѵ�
	result = D3DX11CompileFromFile(
		psFilename,						// ���̴� ���� �̸�
		NULL,
		NULL,
		"LightPixelShader",			// ���̴� �̸�
		"ps_5_0",						// ���̴� ����
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		NULL,
		&pixelShaderBuffer,				// ���̴��� ������ �� ����
		&errorMessage,					// ���� ���ڿ�
		NULL);
	if (FAILED(result))
	{
		// ���̴� �������� �����ϸ� ���� �޼����� ����մϴ�.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
		}
		// ���� �޼����� ���ٸ� �ܼ��� ���̴� ������ ã�� ���� ���Դϴ�.
		else
		{
			MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	// ���ؽ�, �ȼ� ���̴��� �� �����ϵǸ� ���̴� ���۰� �����Ǵµ�
	// �̸� �̿��Ͽ� ���̴� ��ü�� �����Ѵ�
	// ���۷κ��� ���ؽ� ���̴� ��ü ����
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}
	// ���۷κ��� �ȼ� ���̴��� ����
	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	// ���̴����� ����� ���ؽ� �������� ���̾ƿ��� �����Ѵ�
	// ���̴����� ��ġ ���Ϳ� ���� ���͸� ����Ѵ�
	// ���̾ƿ��� ������ ���� ũ�⸦ �����ϴ� �� ���̾ƿ��� �����

	// ���� �Է� ���̾ƿ� description�� �ۼ�
	// �� ������ ModelClass�� ���̴��� �ִ� VertexType�� ��ġ�ؾ� �Ѵ�

	// SemanticName - �� ��Ұ� ���̾ƿ����� ��� ���Ǵ��� �˷��ش�
	// ���ؽ� ���̴����� �Է��� POSITION, TEXCOORD, NORMAL�̹Ƿ� 3�� �̸��� �����Ѵ�
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;

	// ��ġ������ x,y,z�� ���
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;

	// ���ۿ� �����Ͱ� ��� �迭�Ǵ°�
	// 0���� ������
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;



	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;

	// �ؽ��� ������ u,v 2���� ��� ���
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;

	// ��ġ���� ����ϰ� ���� ��� ���������� �ڵ����� ã�´�
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;

	// ���� ������ x, y, z 3���� �����
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;

	// ��ġ���� ����ϰ� ���� ��� ���������� �ڵ����� ã�´�
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	// ���̾ƿ� desc�ۼ� �Ϸ� �� Direct3D ��ġ�� �Է� ���̾ƿ��� �����Ѵ�
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// ���ؽ� �Է� ���̾ƿ��� ����
	result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	// �� �̻� ������ �ʴ� ���� ���̴� �۹��� �ȼ� ���̴� ���۸� ����
	vertexShaderBuffer->Release();
	vertexShaderBuffer = nullptr;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = nullptr;

	// ���̴� ����� ���� ������ �ܰ�� ��� �����̴�
	// ���̴��� ���������� ����, ��, �������

	// ���� ���̴��� �ִ� ��� ��� ������ description�� �ۼ�
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);

	// ��� ���ۿ� �� ���۸� ����Ѵٴ� ���� �����Ѵ�
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// ��� ���� �����͸� ����� �� Ŭ��������
	// ���ؽ� ���̴� ��� ���ۿ� ������ �� �ְ� �Ѵ�
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// SamplerState ����
	// Filter - ���� ���� ǥ�鿡�� �ؽ����� ��� �ȼ��� ���, ȥ�յ� ���ΰ�
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	// 0.0f, 1.0f ���̿� ���� �ֵ��� wrap ����
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

	// SamplerState ����
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}
	
	// ī�޶� ������ desc�� �ۼ��ϰ� ���۸� �����Ѵ�
	// ���ؽ� ���̴����� ī�޶��� ��ġ�� ���� �� �ְ� �Ѵ�
	cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
	cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cameraBufferDesc.MiscFlags = 0;
	cameraBufferDesc.StructureByteStride = 0;
	 
	result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// ���� ��� ������ desc �ۼ��Ѵ�
	// D3D11_BIND_CONSTANT_BUFFER, CreateBuffer ��� ���� ���
	// ������ ũ�Ⱑ 16�� ������� Ȯ���� ��
	// �ȱ׷��� CreateBuffer �Լ��� ������
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	
	// ���� ��� ���� ����
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void LightShaderClass::ShutdownShader()
{
	// ���� ��� ���� ����
	if (m_lightBuffer != nullptr)
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}

	if (m_cameraBuffer != nullptr)
	{
		m_cameraBuffer->Release();
		m_cameraBuffer = nullptr;
	}

	// ���÷� ���¸� �����Ѵ�
	if (m_sampleState != NULL)
	{
		m_sampleState->Release();
		m_sampleState = nullptr;
	}

	// ��� ���۸� �����Ѵ�
	if (m_matrixBuffer != nullptr)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	// ���̾ƿ��� �����Ѵ�
	if (m_layout != nullptr)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	// �ȼ� ���̴��� �����Ѵ�
	if (m_pixelShader != nullptr)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	// ���� ���̴��� �����Ѵ�
	if (m_vertexShader != nullptr)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	return;
}

void LightShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	// ���� �޼����� ��� �ִ� ���ڿ� ������ �����͸� �����ɴϴ�.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// �޼����� ���̸� �����ɴϴ�.
	bufferSize = errorMessage->GetBufferSize();

	// ������ ���� �ȿ� �޼����� ����մϴ�.
	fout.open("shader-error.txt");

	// ���� �޼����� ���ϴ�.
	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// ������ �ݽ��ϴ�.
	fout.close();

	// ���� �޼����� ��ȯ�մϴ�.
	errorMessage->Release();
	errorMessage = nullptr;

	// ������ ������ ������ �˾� �޼����� �˷��ݴϴ�.
	MessageBox(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

// ���̴��� ���� ������ ���� �ٷ� �� �ְ� �Ѵ�
// GraphicsClass���� ������� ��ĵ��� ����Ѵ�
bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	LightBufferType* dataPtr2;

	CameraBufferType* dataPtr3;

	// transpose - ��ġ, ��� ���� ��ȯ�Ѵ�
	// ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �Ѵ�
	// ���̴������� directX -> openGL�� �Ѿ ��쿡�� ��ġ�� ����ϴ� �� - ���� ��ǥ�谡 �ٸ���

	// ���̴����� ��ġ����� ����ϴ� ����
	// ���̴��� ��ȯ ��ũ��, ��ȯ ��ɵ��� �� �ֵ� ��ĵ鿡 �����ϱ� ����
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);

	// ��� ������ ������ �� �� �ֵ��� ��ٴ�
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� �����͸� �����´�
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// ��� ���ۿ� ����� �����Ѵ�
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ��� ������ ����� Ǭ��
	deviceContext->Unmap(m_matrixBuffer, 0);

	// ���ؽ� ���̴������� ��� ������ ��ġ�� �����Ѵ�
	bufferNumber = 0;

	// ���ؽ� ���̴��� ��� ���۸� �ٲ� ������ �����Ѵ�
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

	// ī�޶� ���� ��װ�
	result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr3 = (CameraBufferType*)mappedResource.pData;

	// ī�޶��� ��ġ���� �Է��Ѵ�
	dataPtr3->cameraPosition = cameraPosition;
	dataPtr3->padding = 0.0f;

	deviceContext->Unmap(m_cameraBuffer, 0);

	// ī�޶� ���ؽ� ���̴��� �ι�° �����̹Ƿ� 1�� ����
	bufferNumber = 1;
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

	// �ȼ� ���̴��� �ؽ��ĸ� �����Ѵ�
	// �ؽ��Ĵ� �ݵ�� ���ۿ� �������� �Ͼ�� ���� �����Ǿ� �־���Ѵ�
	deviceContext->PSSetShaderResources(0, 1, &texture);

	// ���� ��� ���۸� ����ϱ� ���� ��ٴ�
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// ���� ������� ������ �����´�
	dataPtr2 = (LightBufferType*)mappedResource.pData;

	// ���� ������ ���۷� �����Ѵ�
	dataPtr2->ambientColor = ambientColor;			// �ֺ���
	dataPtr2->diffuseColor = diffuseColor;			// ����
	dataPtr2->lightDirection = lightDirection;		// ������ ����
	dataPtr2->specularColor = specularColor;		// ���ݻ籤 ��
	dataPtr2->specularPower = specularPower;		// ���ݻ籤 ����

	// ���� ��������� ����� �����Ѵ�
	deviceContext->Unmap(m_lightBuffer, 0);

	bufferNumber = 0;

	// �ȼ� ���̴��� ���� ��� ���۸� �����Ѵ�
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);
	return true;
}

// SetShaderParameters���� ȣ��Ǿ� ���̴��� ���ڵ��� �̿��� �ﰢ���� �׸���
// 1. �Է� ���̾ƿ��� Input assembler�� �����Ѵ�
// �� ����� GPU ���ؽ� ������ �ڷᱸ���� �˰� �ȴ�
// 2. ���ؽ� ���۸� �׸��� ���� ���ؽ�, �ȼ� ���̴��� �����Ѵ�
// 3. �ȼ� ���̴��� ���÷� ���¸� �����Ѵ�
// ���̴��� �����Ǹ� deviceContext���� DrawIndexed �޼ҵ�� �ﰢ���� �׸���
void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 1. ���ؽ� �Է� ���̾ƿ��� �����Ѵ�
	deviceContext->IASetInputLayout(m_layout);

	// 2. �ﰢ���� �׸� ���ؽ�, �ȼ� ���̴��� �����Ѵ�
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// 3. �ȼ� ���̴��� ���÷� ���¸� �����Ѵ�
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// �ﰢ���� �׸���
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
