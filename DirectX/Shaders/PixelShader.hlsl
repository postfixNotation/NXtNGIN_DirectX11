struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}