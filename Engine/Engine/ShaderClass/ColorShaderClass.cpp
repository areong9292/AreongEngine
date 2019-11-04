#include "ColorShaderClass.h"

ColorShaderClass::ColorShaderClass()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_matrixBuffer = nullptr;
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass& other)
{
}


ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// ���̴��� �ʱ�ȭ�ϴ� �޼ҵ�
	// HLSL ���̴� ������ �̸��� �Ѱ��ش�
	result = InitializeShader(device, hwnd, L"../Engine/HLSL/Color.vs", L"../Engine/HLSL/Color.ps");
	if (!result)
	{
		return false;
	}
	return true;
}

void ColorShaderClass::Shutdown()
{
	// ���ؽ� ���̴�, �ȼ� ���̴� ���õ��� ��ȯ�Ѵ�
	ShutdownShader();

	return;
}

bool ColorShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	bool result;

	// �������� ����� ���̴��� ���ڸ� �Է��Ѵ�
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// ���̴��� �غ�� ���۸� �׸���
	RenderShader(deviceContext, indexCount);

	return true;
}

// ���̴� ������ �ҷ����� DirectX�� GPU���� ��� �����ϵ��� �Ѵ�
bool ColorShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	// �� ���̴����� �Է� ��Ҹ� 2���� ����Ѵ�
	// �Է� ��� ���Ϳ� ���� 2���� �Է� ���̾ƿ� ������ �ʿ��ϴ�
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];

	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	
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
						"ColorVertexShader",			// ���̴� �̸�
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
						"ColorPixelShader",				// ���̴� �̸�
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
	// ���ؽ� ���̴����� �Է��� POSITION, COLOR�̹Ƿ� 2�� �̸��� �����Ѵ�
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
	


	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;

	// �÷������� r,g,b,a 4���� ��� ���
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;

	// ��ġ���� ����ϰ� ���� ��� ���������� �ڵ����� ã�´�
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; 
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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
	if(FAILED(result))
	{
		return false;
	}
	
	return true;
}

void ColorShaderClass::ShutdownShader()
{
	// ��� ���۸� �����Ѵ�
	if(m_matrixBuffer != nullptr)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}
	
	// ���̾ƿ��� �����Ѵ�
	if(m_layout != nullptr)
	{
		m_layout->Release();
		m_layout = nullptr;
	}
	
	// �ȼ� ���̴��� �����Ѵ�
	if(m_pixelShader != nullptr)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}
	
	// ���� ���̴��� �����Ѵ�
	if(m_vertexShader != nullptr)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
	
	return;
}

void ColorShaderClass::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
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
	for(i=0; i<bufferSize; i++)
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
bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;
	
	// transpose - ��ġ, ��� ���� ��ȯ�Ѵ�
	// ����� transpose�Ͽ� ���̴����� ����� �� �ְ� �Ѵ�
	// ���̴������� directX -> openGL�� �Ѿ ��쿡�� ��ġ�� ����ϴ� �� - ���� ��ǥ�谡 �ٸ���

	// ���̴����� ��ġ����� ����ϴ� ����
	// ���̴��� ��ȯ ��ũ��, ��ȯ ���ɵ��� �� �ֵ� ��ĵ鿡 �����ϱ� ����
	D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	
	// ��� ������ ������ �� �� �ֵ��� ��ٴ�
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
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
	
	// ���������� ���ؽ� ���̴��� ��� ���۸� �ٲ� ������ �ٲ۴�
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	
	return true;
}

// SetShaderParameters���� ���� ȣ��Ǿ� ���̴��� ���ڵ��� �ùٸ��� �����Ѵ�
// 1. �Է� ���̾ƿ��� Input assembler�� �����Ѵ�
// �� ����� GPU ���ؽ� ������ �ڷᱸ���� �˰� �ȴ�
// 2. ���ؽ� ���۸� �׸��� ���� ���ؽ�, �ȼ� ���̴��� �����Ѵ�
// ���̴��� �����Ǹ� deviceContext���� DrawIndexed �޼ҵ�� �ﰢ���� �׸���
void ColorShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
	// 1. ���ؽ� �Է� ���̾ƿ��� �����Ѵ�
	deviceContext->IASetInputLayout(m_layout);

	// 2. �ﰢ���� �׸� ���ؽ�, �ȼ� ���̴��� �����Ѵ�
	deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	deviceContext->PSSetShader(m_pixelShader, NULL, 0);

	// �ﰢ���� �׸���
	deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}