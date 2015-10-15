Texture2DArray shaderTexture1;
/* : register(t0);//+x
Texture2D shaderTexture2 : register(t1);//-x
Texture2D shaderTexture3 : register(t2);//+y
Texture2D shaderTexture4 : register(t3);//-y
Texture2D shaderTexture5 : register(t4);//+z
Texture2D shaderTexture6 : register(t5);//-z
Texture2D SpecularTexture : register(t6);
*/
SamplerState SampleType;
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

bool is_nearly_eq(float a,float b)
{
  if(abs(a-b)<0.0001) return true;
  return false;
}
float get_g(float3 i,float3 n,float k)
{
	return dot(n,i)/(dot(n,i)*(1-k)+k);
}
float4 PShader(VOut input) : SV_TARGET
{


float conv_f = 1;
  float4 ambient = {0.5,0.5,0.6,1.0};
  //point light
  float range = 100.0;
  float3 pos = {0,12,0};

float3 view =  input.positionw - cam_position;
float3 V = view;
float3 L = light_dir;
float view_dis = length(view);
float cur_texture = 0;
float3 nview =  normalize(view);
float3 ref = (nview - (2*dot(nview,input.normal))*input.normal);
float3 R = ref;
float3 N = normalize(input.normal);
float3 H = normalize(L + V);
//float3 ref = reflect(view,input.normal);
if(bref>0)
{
	view = ref*2;
	conv_f = 3.1415;
}
float z = 0;
float x = 0,y = 0;
float ma = max(max(abs(view.x),abs(view.y)),abs(view.z));

  if(is_nearly_eq(ma,abs(view.x)))
  {
    if(view.x>0)
    {
      cur_texture = 0;
      y = 1-(view.y/view.x + 1)*0.5;
      x = 1-(view.z/view.x + 1)*0.5;
    }
    else
    {
      cur_texture = 1;
      y = (view.y/view.x + 1)*0.5;
      x = 1-(view.z/view.x + 1)*0.5;
    }


  }
  else if(is_nearly_eq(ma,abs(view.y)))
  {
    if(view.y>0)
    {
      cur_texture = 2;
      x = (view.x/view.y + 1)*0.5;
      y = (view.z/view.y + 1)*0.5;
    }
    else
    {
      cur_texture = 3;
      x = 1-(view.x/view.y + 1)*0.5;
      y = (view.z/view.y + 1)*0.5;
    }


  }
  else if(is_nearly_eq(ma,abs(view.z)))
  {
    if(view.z>0)
    {
      cur_texture = 4;
      x = (view.x/view.z + 1)*0.5;
      y = 1-(view.y/view.z + 1)*0.5;
    }
    else
    {
      cur_texture = 5;
      x = (view.x/view.z + 1)*0.5;
      y = (view.y/view.z + 1)*0.5;
    }


  }


  float3 texcs = float3(x,y,cur_texture);
  float4 cs = shaderTexture1.Sample(SampleType,texcs);

	

  //cs = shaderTexture1.Sample(SampleType,float3(input.tex,cur_texture));
//	float4 c0 = shaderTexture1.Sample(SampleType,input.tex);
	//float4 c1 = shaderTexture1.Sample(SampleType,input.tex);
  //cs=cs/conv_f;
	cs.a = 1.0;
	return cs;
}
