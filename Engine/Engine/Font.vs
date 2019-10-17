// 버텍스 쉐이더

// 버텍스 쉐이터에서 쓰일 행렬들
cbuffer MatrixBuffer
{
	matrix worldMatrix;					// 월드 행렬
	matrix viewMatrix;					// 뷰 행렬
	matrix projectionMatrix;			// 투영 행렬
};

// 버텍스 쉐이더의 입력 형태
struct VertexInputType
{
	float4 position : POSITION;			// 좌표 값으로 사용
	float2 tex : TEXCOORD0;				// 텍스쳐 좌표 값으로 사용
};

// 버텍스 쉐이더의 출력 형태
// 버텍스 쉐이더의 출력을 픽셀 쉐이더 입력으로 사용한다
struct PixelInputType
{
	// SV_POSITION에 대한 설명 링크
	// https://www.gamedev.net/forums/topic/579610-hlsl-semantics-position-vs-sv_position/
	float4 position : SV_POSITION;		// 시스템 벨류 좌표
	float2 tex : TEXCOORD0;				// 텍스쳐 좌표 값으로 사용
};

// 정점 쉐이더는 정점 버퍼의 데이터를 처리하기 위해 GPU에 의해 호출된다
PixelInputType FontVertexShader(VertexInputType input)
{
	// 픽셀 쉐이더로 전달할 버텍스 쉐이더의 결과 값
	PixelInputType output;

	input.position.w = 1.0f;

	// 정점의 위치를 월드 변환, 뷰 변환, 투영 변환 순으로 계산
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// 텍스쳐 좌표를 전달한다
	output.tex = input.tex;

	return output;
}