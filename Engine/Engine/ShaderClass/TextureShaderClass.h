#ifndef _TEXTURESHADERCLASS_H_
#define _TEXTURESHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

// GPU�� �����ϴ� 3D �𵨵��� �׸��µ� ����ϴ�
// HLSL ���̴�(.vs, .ps ����)�� ȣ���ϴ� Ŭ����
class TextureShaderClass
{
private:
	// ���ؽ� �����Ϳ� ���� cbuffet ����ü�� ����
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};
public:
	TextureShaderClass();
	TextureShaderClass(const TextureShaderClass& other);
	~TextureShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	// ���̴��� ���Ǵ� �������� �����ϰ�
	// ���̴��� �̿��� �غ�� ���� ���ؽ����� �׷�����
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	ID3D11SamplerState* m_sampleState;
};

#endif