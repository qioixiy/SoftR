Texture2D shaderTexture : register(t0);
SamplerState SampleType;



cbuffer ColorBuffer:register(b3)
{
	float4 light_color;
	float3 light_dir;
	float pading;
	float3 cam_position;

};


struct PSIn
{
	float4 position : SV_POSITION;
	float3 positionw : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

float get_g(float3 i,float3 n,float k)
{
	return dot(n,i)/(dot(n,i)*(1-k)+k);
}

float4 PShader(PSIn input) : SV_TARGET
{

float fog_start = 1;
float fog_range = 20;
float4 fogcolor = {0.5,0.5,0.5,1};


if(input.tex.x>1||input.tex.y>1)
{

		input.tex.x/=5.f;
		input.tex.y/=5.f;
	}
	//point light
	float3 light_pos = float3(1,5,0);
	float kc = 0.8;
	float kl = 0.002;

	float4 gl = {0.9,0.5,0.0,1.0};
	float3 light = light_dir;
	light = normalize(light);
	float3 lc = float3(0.8,0.8,0.8);
 float dis = length(light_pos - input.positionw);
 float3 density = lc/(kl*dis+kc);
float3 view = cam_position - input.positionw;
 //fog
float t = saturate((length(view)-fog_start)/fog_range);

clip(fog_start+fog_range-length(view));

#ifdef NORMALP
	float def = saturate(dot(light,input.normal));
	float4 cc = pow(shaderTexture.Sample(SampleType,input.tex),1.f)*float4(density,1);
	float4 ccc = light_color*cc;


	float3 hv = normalize(light_dir+view);

	float spc = saturate(dot(hv,input.normal));
	if(def==0)
		spc = 0;
	else
		spc = saturate(pow(spc,500)*2);


	ccc = saturate((ccc * def + gl*cc) + ccc*spc*def);
	ccc.a = cc.a;
ccc = pow(ccc,1.f);
ccc = lerp(ccc,fogcolor,t);
	return ccc;
#else
	//以下各向异性着色with经验公式
	float4 cc = pow(shaderTexture.Sample(SampleType,input.tex),1.f)*float4(density,1);

	float3 P = input.positionw.xyz;
	float3 N = normalize(input.normal);
	float3 L = light;
	float ln = max(dot(L,N),0);
	float3 def = 0.01*lc*ln;
	float3 V = normalize(cam_position - P);
	float3 H = normalize(L + V);
	float3 spec = float3(0,0,0);
	bool back = (dot(V,N)>0&&dot(L,N));
	if(back)
	{
		float3 T = normalize(cross(N,V));
		float a = dot(L,T);
		float b = dot(V,T);
		float c = sqrt(1-a*a)*sqrt(1-b*b)-a*b;
		float brdf = 3*pow(c,18.0)/3.141592654;
		spec = brdf*lc*ln;
	}
	float4 oc;
	oc.xyz = gl + def + spec;
	oc.a = 1.f;
	oc = oc*cc;
	oc = lerp(oc,fogcolor,t);
	//Oren-Nayar Diffuse
	float rudness = 0;
	float phi1 = abs(acos(L.x / sqrt(L.x*L.x + L.z*L.z)));
	float phi2 = abs(acos(V.x / sqrt(V.x*V.x + V.z*V.z)));
	float theta1 = abs(acos(L.y));
	float theta2 = abs(acos(V.y));
	float C = max(0,cos(phi1-phi2))*sin(max(theta1,theta2))*tan(min(theta2,theta1));
	float A = 1 - 0.5*(rudness*rudness/(rudness*rudness + 0.57));
	float B = 0.45*(rudness*rudness/(rudness*rudness+0.09));
	oc.xyz = 0.6*ln*(A+(B*C))*lc+gl;
	oc.w = 1.0;
	//Minnaert
	float e = 2.7183;
	oc.xyz = 0.6*pow((dot(N,L)*dot(N,V)),2-1)*lc + gl;
	//ward iso
	float wsig = 1.0;
	float wgama = acos(H);
	float wsxpec = 1.0;
	float wtemp = wsxpec*pow(e,
		-pow(
			tan(wgama/(wsig*wsig))
			,2)
			)/(2*3.14159*wsig*wsig*sqrt(abs(dot(N,L))*dot(N,V)));
	oc.xyz = (wtemp+0.6)*lc*ln+gl;
	//ward ani
	float wsigx = 0.1;
	float wsigy = 0.5;
	wtemp = wsxpec*pow(e,
		-pow(
			tan(wgama*(cos(phi1)*cos(phi1)/(wsigx*wsigx) + sin(phi1)*sin(phi1)/(wsigy*wsigy)))
			,2)
			)/(2*3.14159*wsigy*wsigx*sqrt(dot(N,L)*dot(N,V)));
	oc.xyz = (wtemp+0.6)*lc*ln+gl;
	//Schick
	float3 R = 2*N*dot(L,N) - L;
	float sn = 130;
	oc.xyz = ln*lc*0.6 + wsxpec*lc*(dot(R,V)/(sn-((sn-1)*dot(R,V)))) + gl;
	//Cook-Torrance UE4
	float rn = 1.3;
	float al = rn*rn;
	/*
	float pram_yita = 6.1;
	float cG = min(1,2*dot(N,H)*dot(N,V)/dot(V,H));
	cG = min(cG,2*dot(N,H)*dot(N,L)/dot(V,H));
	*/
	float mk = pow((rn+1),2)/8;
	float cG = get_g(N,V,mk)*get_g(N,L,mk);
	/*
	float cFc = dot(V,H);
	float cFg = sqrt(pram_yita*pram_yita+cFc*cFc-1);
	float cF = (pow(cFg-cFc,2)/(2*pow(cFg+cFc,2))) *
	(1+(pow((cFc*(cFg+cFc)-1),2)/pow((cFc*(cFg+cFc)+1),2)));
	*/
	float F0 = 0.8;
	float cF = F0 + (1-F0)*pow(2,(-5.55473*dot(V,H) - 6.98316)*(V,H));
	/*
	float cDc = 0.5;
	float cD = cDc*pow(
		e,
		-pow(acos(dot(N,H))/0.5,2)
		);
*/
	float cD = al*al/(3.1415*pow(dot(N,H)*dot(N,H)*(al*al-1)+1,2));
	oc.xyz = 0.1*lc*(cF*cD*cG/(4*(dot(N,L)*dot(N,V)))) + gl;
//	oc = lerp(oc,fogcolor,t);
	return oc;
#endif
}
