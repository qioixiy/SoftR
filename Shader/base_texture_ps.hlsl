Texture2D shaderTexture;
SamplerState SampleType;

cbuffer ColorBuffer:register(b2)
{
	float4 c;
};

/*
struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
*/
struct PSIn
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 PShader(PSIn input) : SV_TARGET
{
	float4 oc;
	oc =  shaderTexture.Sample(SampleType, input.tex);

  return c;
}
