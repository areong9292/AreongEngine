// ���ؽ� ���̴�

// ���ؽ� �����Ϳ��� ���� ��ĵ�
cbuffer MatrixBuffer
{
	matrix worldMatrix;					// ���� ���
	matrix viewMatrix;					// �� ���
	matrix projectionMatrix;			// ���� ���
};

// ���ؽ� ���̴��� �Է� ����
struct VertexInputType
{
	float4 position : POSITION;			// ��ǥ ������ ���
	float2 tex : TEXCOORD0;				// �ؽ��� ��ǥ ������ ���
};

// ���ؽ� ���̴��� ��� ����
// ���ؽ� ���̴��� ����� �ȼ� ���̴� �Է����� ����Ѵ�
struct PixelInputType
{
	// SV_POSITION�� ���� ���� ��ũ
	// https://www.gamedev.net/forums/topic/579610-hlsl-semantics-position-vs-sv_position/
	float4 position : SV_POSITION;		// �ý��� ���� ��ǥ
	float2 tex : TEXCOORD0;				// �ؽ��� ��ǥ ������ ���
};

// ���� ���̴��� ���� ������ �����͸� ó���ϱ� ���� GPU�� ���� ȣ��ȴ�
PixelInputType FontVertexShader(VertexInputType input)
{
	// �ȼ� ���̴��� ������ ���ؽ� ���̴��� ��� ��
	PixelInputType output;

	input.position.w = 1.0f;

	// ������ ��ġ�� ���� ��ȯ, �� ��ȯ, ���� ��ȯ ������ ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// �ؽ��� ��ǥ�� �����Ѵ�
	output.tex = input.tex;

	return output;
}