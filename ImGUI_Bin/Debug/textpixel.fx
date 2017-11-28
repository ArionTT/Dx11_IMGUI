
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ConstantBuffer : register( b0 )
{
	matrix world;
	matrix view;
	matrix projection;
	float4 textColor;
	float4 bgColor;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 uv:TEXCOORD0;
};


float4  pixel(PS_INPUT input):SV_Target
{
  //  float4 ret;
  //  float4 color=shaderTexture.Sample(SampleType,input.uv);
  //  if(color.a==0.0)
		//ret=float4(bgColor);
  //  else
		//ret=float4(color);

  //  return ret;

	float4 color = shaderTexture.Sample(SampleType,input.uv);
	return color;
}