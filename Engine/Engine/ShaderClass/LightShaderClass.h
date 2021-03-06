#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <d3dcompiler.h>
using namespace std;
using namespace DirectX;

// GPU상에 존재하는 3D 모델들을 그리는데 사용하는
// HLSL 쉐이더(.vs, .ps 파일)을 호출하는 클래스
class LightShaderClass
{
private:
	// 버텍스 쉐이터에 사용될 cbuffet 구조체의 정의
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	// 카메라 위치 저장용
	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	// 조명 정보 저장용
	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};
public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass& other);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	// 쉐이더에 사용되는 변수들을 설정하고
	// 쉐이더를 이용해 준비된 모델의 버텍스들을 그려낸다
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;

	// 카메라 정보를 위한 버퍼
	ID3D11Buffer* m_cameraBuffer;

	// 빛 정보를 위한 버퍼
	ID3D11Buffer* m_lightBuffer;
};

#endif