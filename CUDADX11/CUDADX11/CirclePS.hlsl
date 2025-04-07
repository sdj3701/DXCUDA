Texture2D baseColorTextur : register(t0);
SamplerState baseColorSampler : register(s0);

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(VSOutput vsOutput) : SV_TARGET
{
    return baseColorTextur.Sample(baseColorSampler, vsOutput.uv);
}