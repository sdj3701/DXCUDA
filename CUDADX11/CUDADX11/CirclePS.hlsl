// CirclePS.hlsl
Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

// �� ������ ��� ��� ����
cbuffer CircleData : register(b0)
{
    float2 center; // �߽� ��ǥ (��ũ�� ��ǥ)
    float radius; // ������
    float padding; // �е�
    float4 color; // �� ����
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput vsOutput) : SV_TARGET
{
    // ���� �ȼ��� ��ũ�� ��ǥ ���
    float2 screenPos = vsOutput.position.xy;
    
    // �߽ɿ��� ���� �ȼ������� �Ÿ� ���
    float distance = length(screenPos - center);
    
    // �� �ȿ� �ִ��� Ȯ��
    if (distance <= radius)
    {
        // �� ���ζ�� ������ ���� ��ȯ
        return color;
    }
    else
    {
        // �� �ܺζ�� �ؽ�ó ���� �Ǵ� ����� ��ȯ
        return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
    }
}