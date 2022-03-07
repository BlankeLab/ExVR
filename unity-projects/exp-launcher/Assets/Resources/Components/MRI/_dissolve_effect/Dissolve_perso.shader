	Shader "Custom/Dissolve_perso" {
	Properties{
	_Color("Color", Color) = (1,1,1,1)
	_MainTex("Albedo (RGB)", 2D) = "white" {}

	[Enum(Metallic Alpha,0,Albedo Alpha,1)] _SmoothnessTextureChannel("Smoothness texture channel", Float) = 0

	_MetallicGlossMap("Metallic", 2D) = "white" {}
	_Glossiness("Smoothness", Range(0.0, 1.0)) = 0.1

	_BumpMap("Normal Map", 2D) = "bump" {}

	_OcclusionMap("Occlusion", 2D) = "white" {}
	_OcclusionStrength("Occlusion Strength", Range(0.0, 1.0)) = 1.0

	_SliceGuide("Slice Guide (RGB)", 2D) = "white" {}
	_SliceAmount("Slice Amount", Range(0.0, 1.0)) = 0

	_BurnSize("Burn Size", Range(0.0, 1.0)) = 0.15
	_BurnRamp("Burn Ramp (RGB)", 2D) = "white" {}
	_BurnColor("Burn Color", Color) = (1,1,1,1)

	_EmissionAmount("Emission amount", float) = 2.0


	}

	SubShader{
	Tags{ "RenderType" = "Opaque" }
	LOD 200
	Cull Off

	CGPROGRAM
	#pragma surface surf Standard fullforwardshadows
	#pragma target 3.0

	fixed4 _Color;
	sampler2D _MainTex;
	sampler2D _BumpMap;
	sampler2D _MetallicGlossMap;
	sampler2D _OcclusionMap;
	sampler2D _SliceGuide;
	sampler2D _BurnRamp;

	fixed4 _BurnColor;

	float _OcclusionStrength;

	struct Input {
		float2 uv_MainTex;
		float2 uv_BumpMap;
		float2 uv_MetallicGlossMap;
		float2 uv_OcclusionMap;
	};

	half _Glossiness;
	half _Metallic;

	half _SliceAmount;
	half _BurnSize;
	half _EmissionAmount;

	void surf(Input IN, inout SurfaceOutputStandard o) {
		// albedo color map
		fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Color;

		// solve-dissolve effect
		half test = tex2D(_SliceGuide, IN.uv_MainTex).rgb - _SliceAmount;
		clip(test);
		if (test < _BurnSize && _SliceAmount > 0) {
			o.Emission = tex2D(_BurnRamp, float2(test * (1 / _BurnSize), 0)) * _BurnColor * _EmissionAmount;
		}

		o.Albedo = c.rgb * _Color.rgb;
		// Apply normal mapping.
		o.Normal = UnpackNormal(tex2D(_BumpMap, IN.uv_BumpMap));
		// metallic map
		fixed4 metal = tex2D(_MetallicGlossMap, IN.uv_MetallicGlossMap);
		o.Metallic = metal.r;
		// occlusion map
		fixed4 occlu = tex2D(_OcclusionMap, IN.uv_OcclusionMap);
		o.Occlusion = (1 - _OcclusionStrength) * occlu.a + _OcclusionStrength * occlu.r;
		// smoothness in albedo alpha canal
		o.Smoothness = _Glossiness * c.a;
		o.Alpha = c.a;
	}

	ENDCG
	}
	FallBack "Diffuse"

}