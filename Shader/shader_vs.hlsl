cbuffer MatrixBuffer:register(b1)
{
  matrix m;
  matrix v;
  matrix p;
};

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};
struct VOut
{
  float4 position : SV_POSITION;
  float4 color : COLOR;
	float3 normal:NORMAL;
	float2 tex : TEXCOORD;
};
VOut VShader(VSIn input)
{
  VOut output;
	float4 mo = float4(input.position,1);
	mo = mul(mo,m);
	//mo = mul(mo,v);
  output.position = mo;
	//mo = mul(mo,p);
	//output.position = mo;
  output.color = input.color;
	output.normal = mul(input.normal,(float3x3)m);
	output.normal = normalize(output.normal);
	output.tex = input.tex;
    return output;
}

struct GOut
{
  float4 position : SV_POSITION;
    float3 positionw : POSITION;
  float4 color : COLOR;
	float3 normal:NORMAL;
	float2 tex : TEXCOORD;
};

cbuffer GSBuffer:register(b2)
{
  int b_generate_normal;
}

[maxvertexcount(12)]
void GShader(triangle VOut gin[3],inout TriangleStream<GOut> stream)
{

if(b_generate_normal==0)
{
  for(int i=0;i<3;++i)
  {
    float4 position_w = gin[i].position;
    float4 position_n1 = position_w + float4(gin[i].normal*0.1f,0);
    float4 position_n2 = position_w + float4(gin[i].normal*0.3f,0);
    GOut v1,v2,v3;
    v1.position = gin[i].position;
	v1.positionw = v1.position;
    v1.color = float4(1,0,0,1);
    v1.normal = gin[i].normal;
    v1.normal = mul(v1.normal,(float3x3)m);
    v1.position = mul(v1.position,v);
    v1.position = mul(v1.position,p);
	v1.tex = float2(0,0);

    v2.position = position_n1;
	v2.positionw = v2.position;
    v2.color = float4(1,0,0,1);
    v2.normal = gin[i].normal;
    v2.normal = mul(v2.normal,(float3x3)m);
    v2.position = mul(v2.position,v);
    v2.position = mul(v2.position,p);
	v2.tex = float2(0,0);

    v3.position = position_n2;
	v3.positionw = v3.position;
    v3.color = float4(1,0,0,1);
    v3.normal = gin[i].normal;
    v3.normal = mul(v3.normal,(float3x3)m);
    v3.position = mul(v3.position,v);
    v3.position = mul(v3.position,p);
	v3.tex = float2(0,0);

    stream.Append(v1);
    stream.Append(v2);
    stream.Append(v3);
    stream.RestartStrip();
  }
}
  for(int i=0;i<3;++i)
  {
    GOut v1;
    v1.position = gin[i].position;
	v1.color = gin[i].color;
	v1.normal = gin[i].normal;
	v1.positionw = (float3)v1.position;
    v1.position = mul(v1.position,v);
    v1.position = mul(v1.position,p);
	v1.tex = gin[i].tex;
    stream.Append(v1);

  }
  stream.RestartStrip();
}
