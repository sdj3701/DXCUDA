// CirclePS.hlsl
Texture2D baseColorTexture : register(t0);
SamplerState baseColorSampler : register(s0);

// 원 정보를 담는 상수 버퍼
cbuffer CircleData : register(b0)
{
    float2 center; // 중심 좌표 (스크린 좌표)
    float radius; // 반지름
    float padding; // 패딩
    float4 color; // 원 색상
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput vsOutput) : SV_TARGET
{
    // 현재 픽셀의 스크린 좌표 계산
    float2 screenPos = vsOutput.position.xy;
    
    // 중심에서 현재 픽셀까지의 거리 계산
    float distance = length(screenPos - center);
    
    // 원 안에 있는지 확인
    if (distance <= radius)
    {
        // 원 내부라면 지정된 색상 반환
        return color;
    }
    else
    {
        // 원 외부라면 텍스처 색상 또는 투명색 반환
        return baseColorTexture.Sample(baseColorSampler, vsOutput.uv);
    }
}