#ifndef _TEXTCLASS_H_
#define _TEXTCLASS_H_

#include "FontClass.h"
#include "FontShaderClass.h"

// 2d 문자열을 화면에 그리는 클래스
class TextClass
{
private:
	// 각 문장의 렌더링 정보를 저장
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int, D3DXMATRIX);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX);
	bool SetMousePosition(int, int, ID3D11DeviceContext*);

	bool SetFps(int, ID3D11DeviceContext*);
	bool SetCpu(int, ID3D11DeviceContext*);
	bool SetRenderCount(int renderCount, ID3D11DeviceContext* deviceContext);
	bool SetIntersection(bool isPick, ID3D11DeviceContext * deviceContext, char* modelName = "");

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX);

private:
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	D3DXMATRIX m_baseViewMatrix;

	// TextClass의 SentenceType을 풀로 관리해보자
	// 지금 로직 상 새로운 문자를 찍으려면 새로 변수를 지정해줘야하는데
	// 아쓰 풀 관리 하듯이 한번 변경해보자
	// -> 엔진에서 스트링 관리할 때 이렇게 하...나?
	SentenceType* m_sentence1;
	SentenceType* m_sentence2;

	// 풀 관리 테스트
	SentenceType* m_sentence;
};

#endif