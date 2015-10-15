cbuffer MatrixBuffer:register(b1)
{
  matrix m;
  matrix v;
  matrix o;
};

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};
cbuffer ColorBuffer:register(b3)
{
	float4 light_color;
	float3 light_dir;
	int bref;
	float3 cam_position;

};

struct VOut
{
  float4 position : SV_POSITION;
  float3 positionw:POSITION;
  float4 color : COLOR;
	float3 normal:NORMAL;
	float2 tex : TEXCOORD;
};

VOut VShader(VSIn input)
{
  VOut output;
	float4 mo = float4(input.position,1);
	mo = mul(mo,m);
  output.positionw = mo.xyz;
	mo = mul(mo,v);
	mo = mul(mo,o);
  //保证天空盒不会被剔除，保证其在摄像机最后面，所有摄像机内的物体都不会再他的后面，
  //如果要永远走不出的天空盒也许需要其他技术，对象总事跟着摄像机移动是个好办法
  if(bref<=0)
    mo.z = mo.w;
	output.position = mo;

  output.color = input.color;
	output.normal = mul(input.normal,(float3x3)m);
	output.normal = normalize(output.normal);

	output.tex = input.tex;

  return output;
}
