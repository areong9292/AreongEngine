#ifndef _FONTSHADERCLASS_H_
#define _FONTSHADERCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <D3DX11async.h>
#include <fstream>
using namespace std;

// GPU상에 존재하는 3D 모델들을 그리는데 사용하는
// HLSL 쉐이더(.vs, .ps 파일)을 호출하는 클래스
class FontShaderClass
{
private:
	// 버텍스 쉐이터에 사용될 cbuffet 구조체의 정의
	struct ConstantBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	// 글자 색상 지정용 구조체
	struct PixelBufferType
	{
		D3DXVECTOR4 pixelColor;
	};

public:
	FontShaderClass();
	FontShaderClass(const FontShaderClass& other);
	~FontShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	// 쉐이더에 사용되는 변수들을 설정하고
	// 쉐이더를 이용해 준비된 모델의 버텍스들을 그려낸다
	bool Render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor);

private:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, D3DXVECTOR4 pixelColor);
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