// ���ؽ� ���̴�

// ���ؽ� �����Ϳ��� ���� ��ĵ�
cbuffer MatrixBuffer
{
	matrix worldMatrix;					// ���� ���
	matrix viewMatrix;					// �� ���
	matrix projectionMatrix;			// ���� ���
};

// ���ݻ� ����� ���� ī�޶��� ��ġ
cbuffer CameraBuffer
{
	float3 cameraPosition;
	float padding;
};

// ���ؽ� ���̴��� �Է� ����
struct VertexInputType
{
	float4 position : POSITION;			// ��ǥ ������ ���
	float2 tex : TEXCOORD0;				// �ؽ��� ��ǥ ������ ���
	float3 normal : NORMAL;				// ��������
};

// ���ؽ� ���̴��� ��� ����
// ���ؽ� ���̴��� ����� �ȼ� ���̴� �Է����� ����Ѵ�
struct PixelInputType
{
	// SV_POSITION�� ���� ���� ��ũ
	// https://www.gamedev.net/forums/topic/579610-hlsl-semantics-position-vs-sv_position/
	float4 position : SV_POSITION;		// �ý��� ���� ��ǥ
	float2 tex : TEXCOORD0;				// �ؽ��� ��ǥ ������ ���
	float3 normal : NORMAL;				// ��������
	float3 viewDirection : TEXCOORD1;	// ���� ����
};

// ���� ���̴��� ���� ������ �����͸� ó���ϱ� ���� GPU�� ���� ȣ��ȴ�
PixelInputType LightVertexShader(VertexInputType input)
{
	// �ȼ� ���̴��� ������ ���ؽ� ���̴��� ��� ��
	PixelInputType output;
	float4 worldPosition;

	input.position.w = 1.0f;

	// ������ ��ġ�� ���� ��ȯ, �� ��ȯ, ���� ��ȯ ������ ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// �ؽ��� ��ǥ�� �����Ѵ�
	output.tex = input.tex;

	// ���� ��ǥ��� ��ȯ
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// �븻 ���� ����ȭ�Ѵ�
	output.normal = normalize(output.normal);

	// ���� ���� ���
	// ������ ������� ��ġ�� ����Ѵ�(���� ��ȯ)
	worldPosition = mul(input.position, worldMatrix);

	// ���� ī�޶� ��ġ���� ���� ���� ��ġ�� ���� ��� �������� �ٶ󺸴��� ���´�
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// �ȼ� ���̴��� ������ ���� ����ȭ���� ������
	output.viewDirection = normalize(output.viewDirection);

	return output;
}