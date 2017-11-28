struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 color:TEXCOORD0;
};


float4  pixel(PS_INPUT input):SV_Target
{
    return input.color;
}