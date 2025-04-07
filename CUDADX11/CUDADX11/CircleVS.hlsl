struct VSInput
{
    float4 position : POSITIONT;
    float2 uv : TEXTCOORD;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXTCOORD;
};


float4 main(VSInput vsInput) 
{
    VSOutput vsOutput;
    
    vsOutput.position = vsInput.position;
    vsOutput.uv = vsInput.uv;
    
    return vsOutput;
}