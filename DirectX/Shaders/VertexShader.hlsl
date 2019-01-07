struct VOut
{
    float4 position : SV_POSITION;
    float3 color : COLOR0;
};

VOut VShader(float4 position : POSITION, float3 color : COLOR0)
{
    VOut output;

    output.position = position;
    output.color = color;

    return output;
}