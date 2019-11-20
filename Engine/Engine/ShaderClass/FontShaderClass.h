#ifndef _FONTSHADERCLASS_H_
#define _FONTSHADERCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <d3dcompiler.h>
using namespace std;
using namespace DirectX;

// GPU�� �����ϴ� 3D �𵨵��� �׸��µ� ����ϴ�
// HLSL ���̴�(.vs, .ps ����)�� ȣ���ϴ� Ŭ����
class FontShaderClass
{
private:
	// ���ؽ� �����Ϳ� ���� cbuffet ����ü�� ����
	struct ConstantBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// ���� ���� ������ ����ü
	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};

public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass& other);
	~FontShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	// ���̴��� ���Ǵ� �������� �����ϰ�
	// ���̴��� �̿��� �غ�� ���� ���ؽ����� �׷�����
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT4 pixelColor);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11SamplerState* m_sampleState;

	ID3D11Buffer* m_pixelBuffer;
};

#endif