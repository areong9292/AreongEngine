// 버텍스 쉐이더

// 버텍스 쉐이터에서 쓰일 행렬들
cbuffer MatrixBuffer
{
	matrix worldMatrix;					// 월드 행렬
	matrix viewMatrix;					// 뷰 행렬
	matrix projectionMatrix;			// 투영 행렬
};

// 정반사 계산을 위한 카메라의 위치
cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

// 버텍스 쉐이더의 입력 형태
struct VertexInputType
{
	float4 position : POSITION;			// 좌표 값으로 사용
	float2 tex : TEXCOORD0;				// 텍스쳐 좌표 값으로 사용
	float3 normal : NORMAL;				// 법선벡터
};

// 버텍스 쉐이더의 출력 형태
// 버텍스 쉐이더의 출력을 픽셀 쉐이더 입력으로 사용한다
struct PixelInputType
{
	// SV_POSITION에 대한 설명 링크
	// https://www.gamedev.net/forums/topic/579610-hlsl-semantics-position-vs-sv_position/
	float4 position : SV_POSITION;		// 시스템 벨류 좌표
	float2 tex : TEXCOORD0;				// 텍스쳐 좌표 값으로 사용
	float3 normal : NORMAL;				// 법선벡터
	float3 viewDirection : TEXCOORD1;	// 보는 방향
};

// 정점 쉐이더는 정점 버퍼의 데이터를 처리하기 위해 GPU에 의해 호출된다
PixelInputType LightVertexShader(VertexInputType input)
{
	// 픽셀 쉐이더로 전달할 버텍스 쉐이더의 결과 값
	PixelInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;

	// 정점의 위치를 월드 변환, 뷰 변환, 투영 변환 순으로 계산
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// 텍스쳐 좌표를 전달한다
	output.tex = input.tex;

	// 월드 좌표계로 변환
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// 노말 벡터 정규화한다
	output.normal = normalize(output.normal);

	// 보는 방향 계산
	// 정점의 월드상의 위치를 계산한다(월드 변환)
	worldPosition = mul(input.position, worldMatrix);

	// 현재 카메라 위치에서 월드 상의 위치를 빼면 어느 방향으로 바라보는지 나온다
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// 픽셀 쉐이더로 보내기 전에 정규화시켜 보낸다
	output.viewDirection = normalize(output.viewDirection);

	return output;
}