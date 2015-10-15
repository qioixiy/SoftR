cbuffer MatrixBuffer:register(b1)
{
  matrix m;
  matrix v;
  matrix o;
};

struct VSIn
{
	float3 position : POSITION;
//	float4 color : COLOR;
//	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};


struct VOut
{
  float4 position : SV_POSITION;
  //float4 color : COLOR;
	//float3 normal:NORMAL;
	float2 tex : TEXCOORD;
};

VOut VShader(VSIn input)
{
  VOut output;
	float4 mo = float4(input.position,1);
	mo = mul(mo,m);
	mo = mul(mo,v);
	mo = mul(mo,o);
	output.position = mo;
/*
  output.color = input.color;
	output.normal = mul(input.normal,(float3x3)m);
	output.normal = normalize(output.normal);
*/
	output.tex = input.tex;

  return output;
}

Texture2D shaderTexture : register(t0);
SamplerState SampleType;

float4 PShader(VOut input) : SV_TARGET
{
	float4 cc = shaderTexture.Sample(SampleType,input.tex);
  /*
  if(input.tex.x > 1.1f || input.tex.y > 1.1f)
    cc.r = 1.f;
    */
  return cc;
}
