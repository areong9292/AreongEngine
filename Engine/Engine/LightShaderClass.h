#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

// GPU�� �����ϴ� 3D �𵨵��� �׸��µ� ����ϴ�
// HLSL ���̴�(.vs, .ps ����)�� ȣ���ϴ� Ŭ����
class LightShaderClass
{
private:
	// ���ؽ� �����Ϳ� ���� cbuffet ����ü�� ����
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	// ī�޶� ��ġ �����
	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};

	// ���� ���� �����
	struct LightBufferType
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightDirection;
		float specularPower;
		D3DXVECTOR4 specularColor;
	};
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	// ���̴��� ���Ǵ� �������� �����ϰ�
	// ���̴��� �̿��� �غ�� ���� ���ؽ����� �׷�����
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR3 lightDirection, D3DXVECTOR4 ambientColor, D3DXVECTOR4 diffuseColor, D3DXVECTOR3 cameraPosition, D3DXVECTOR4 specularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	// ī�޶� ������ ���� ����
	ID3D11Buffer* m_cameraBuffer;

	// �� ������ ���� ����
	ID3D11Buffer* m_lightBuffer;
};

#endif