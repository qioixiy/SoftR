cbuffer MatrixBuffer:register(b1)
{
  matrix m;
  matrix v;
  matrix p;
};

cbuffer ColorBuffer:register(b2)
{
	float4 c;
};

struct VSIn
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct VOut
{
    float4 position : SV_POSITION;
    float2 tex:TEXCOORD0;
};

VOut VShader(VSIn input)
{
    VOut output;

	float4 mo = input.position;
	mo = mul(mo,m);
	mo = mul(mo,v);
	mo = mul(mo,p);
	output.position = mo;
	
	output.tex = input.tex;

    return output;
}

