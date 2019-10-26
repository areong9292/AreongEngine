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
	float4 color : COLOR;				// �÷� ������ ���
};

// ���ؽ� ���̴��� ��� ����
// ���ؽ� ���̴��� ����� �ȼ� ���̴� �Է����� ����Ѵ�
struct PixelInputType
{
	// SV_POSITION�� ���� ���� ��ũ
	// https://www.gamedev.net/forums/topic/579610-hlsl-semantics-position-vs-sv_position/
	float4 position : SV_POSITION;		// �ý��� ���� ��ǥ
	float4 color : COLOR;				// �÷�
};

// ���� ���̴��� ���� ������ �����͸� ó���ϱ� ���� GPU�� ���� ȣ��ȴ�
PixelInputType ColorVertexShader(VertexInputType input)
{
	// �ȼ� ���̴��� ������ ���ؽ� ���̴��� ��� ��
	PixelInputType output;

	input.position.w = 1.0f;

	// ������ ��ġ�� ���� ��ȯ, �� ��ȯ, ���� ��ȯ ������ ���
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// �Է� ������ �����Ѵ�
	output.color = input.color;

	return output;
}