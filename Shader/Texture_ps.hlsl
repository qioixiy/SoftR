Texture2D shaderTexture1 : register(t0);
Texture2D shaderTexture2 : register(t1);
Texture2D shaderTexture3 : register(t2);


Texture2D LightTexture : register(t3);
Texture2D AlphaTexture : register(t4);
Texture2D NormalTexture : register(t5);

Texture2D SpecularTexture : register(t6);



SamplerState SampleType;

struct VOut
{
  float4 position : SV_POSITION;
    float3 positionw:POSITION;
  //float4 color : COLOR;
	//float3 normal:NORMAL;
	float2 tex : TEXCOORD;
};

float4 PShader(VOut input) : SV_TARGET
{

  float4 ambient = {0.5,0.5,0.6,1.0};
  //point light
  float range = 100.0;
  float3 pos = {0,12,0};



	float4 c0 = shaderTexture1.Sample(SampleType,input.tex);
	float4 c1 = shaderTexture2.Sample(SampleType,input.tex);
	float2 t = input.tex/4;
	float4 c2 = shaderTexture3.Sample(SampleType,input.tex);


  /*
  if(input.tex.x > 1.1f || input.tex.y > 1.1f)
    cc.r = 1.f;
    */

	float4 cc = c1*c0*2;



  return cc;
}
