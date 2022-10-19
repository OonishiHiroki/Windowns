#include"Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1));
	//float brightness = dot(-light, input.normal);
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 texcolor = float4(tex.Sample(smp,input.uv));
	//RGB�����ꂼ��@����XYZ,A��1�ŏo��
	//return float4(brightness, brightness, brightness, 1);
	//return float4(1,1,1,1);
	return float4(texcolor.rgb * brightness, texcolor.a) * color;
}