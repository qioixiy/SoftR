
Texture2D diff : register(t0);
Texture2D normal : register(t1);
Texture2D uvks : register(t2);
Texture2D color : register(t3);
SamplerState SampleType;


cbuffer ColorBuffer:register(b3)
{
	float4 light_color;
	float3 light_dir;
	float pading;
	float3 cam_position;

};

//G-Buffer
struct PS_OUT
{
  float4 position:SV_TARGET0;
  //uv,漫反射系数，高光系数
  float4 uv_kd_spe:SV_TARGET1;
  float4 normal:SV_TARGET2;
  float4 color:SV_TARGET3;
}

struct PSIn
{
	float4 position : SV_POSITION;
	float3 positionw : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

PS_OUT PShaderGeo(PSIn input):SV_TARGET
{
  PS_OUT output;
  output.position = input.positionw;
  output.normal = input.normal;
  output.color = shaderTexture.Sample(SampleType,input.tex);
  output.uv_kd_spe = float4(input.tex,1,0,1.0);
  return output;
}

//Shading
float4 PShaderShading(PSIn input) : SV_TARGET
{
  
	float4 gl = {0.5,0.5,0.5,1.0};
	float3 light = light_dir;
	light = normalize(light);
	float def = saturate(dot(light,input.normal));
	float4 cc = shaderTexture.Sample(SampleType,input.tex);
	float4 ccc = light_color*cc;

	float3 view = cam_position - input.positionw;
	float3 hv = normalize(light_dir+view);

	float spc = saturate(dot(hv,input.normal));
	if(def==0)
		spc = 0;
	else
		spc = saturate(pow(spc,50)*2);


	ccc = saturate((ccc * def + gl*cc) + ccc*spc*def);
	ccc.a = cc.a;

	return ccc;
}
