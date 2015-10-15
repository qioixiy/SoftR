#include "ShaderPixel.h"
#include "Texture2D.h"
#include "SamplePoint.h"

class BaseShaderPS : public SrShaderPixel
{
public:
	RBColorf shade(VertexP3N3T2& vert_lerp)
	{
		SrTexture2D* tex = get_texture2d("texture");
		SrSamplerPoint sp;
		RBColorf tc = sp.sample(tex, vert_lerp.text_coord.x, vert_lerp.text_coord.y);


		vert_lerp.normal.normalize();
		//vert_lerp.normal = vert_lerp.normal.get_abs();

		RBVector3 v = vert_lerp.position;
		v = v.get_abs();
		v.normalize();

		RBVector3 light(-1,-1,-1);
		float cost = RBVector3::dot_product(vert_lerp.normal, light);

		RBColorf oc = RBColorf::white;
		if (cost <= 0)
			oc = RBColorf::black;
		RBVector4 outc = oc;
		outc = outc * cost;
		outc.w = 1.0;

		if (outc.x > 1) outc.x = 1.f;
		if (outc.y > 1) outc.y = 1.f;
		if (outc.z > 1) outc.z = 1.f;

		if (tc.a < 0.0001)
			RBColorf f = RBColorf::black;


		RBColorf c =outc* tc;
		//if (tc.r < 0.0001&&tc.g < 0.0001&&tc.b < 0.0001&&tc.a < 1)
			//c = RBColorf::black;
		//c = RBVector4(vert_lerp.normal, 1);
		//c = RBColorf::red;

		return c;
	}

private:



};