Shader "Custom/Mirror"
{
	Properties
	{
		_MainTex("Emissive Texture", 2D) = "black" {}
		_DetailTex("Detail Texture", 2D) = "white" {}
		_Color("Detail Tint Color", Color) = (1,1,1,1)
		_SpecColor("Specular Color", Color) = (1,1,1,1)
		_SpecularArea("Specular Area", Range(0, 0.99)) = 0.1
		_SpecularIntensity("Specular Intensity", Range(0, 1)) = 0.75
		_ReflectionColor("Reflection Tint Color", Color) = (1,1,1,1)
	}
		SubShader
		{
			Tags { "RenderType" = "Opaque" }
			LOD 300

			CGPROGRAM

			#pragma surface surf BlinnPhong fullforwardshadows
			#pragma multi_compile __ MIRROR_RECURSION
			#include "UnityCG.cginc"

			fixed4 _Color;
			fixed4 _ReflectionColor;
			half _SpecularArea;
			half _SpecularIntensity;
			sampler2D _DetailTex;
			sampler2D _MainTex;

			struct Input
			{
				float2 uv_DetailTex;
				float4 screenPos;
			};

			void surf(Input IN, inout SurfaceOutput o)
			{
				fixed4 detail = tex2D(_DetailTex, IN.uv_DetailTex);

				#if MIRROR_RECURSION

				fixed4 refl = tex2D(_MainTex, IN.uv_DetailTex);

				#else

				IN.screenPos.w = max(0.001, IN.screenPos.w);
				fixed4 refl = tex2Dproj(_MainTex, UNITY_PROJ_COORD(IN.screenPos));

				#endif

				o.Albedo = detail.rgb * _Color.rgb;
				o.Alpha = 1;
				o.Specular = 1.0f - _SpecularArea;
				o.Gloss = _SpecularIntensity;
				o.Emission = refl.rgb * _ReflectionColor.rgb;
			}

			ENDCG
		}

			FallBack "Reflective/Specular"
}


//
//
//
//Shader "Custom/MirrorSPI" {
//
//    Properties{
//        _MainTex("Emissive Texture", 2D) = "black" { }
//        _DetailTex("Detail Texture", 2D) = "white" { }
//        _Color("Detail Tint Color", Color) = (1.000000,1.000000,1.000000,1.000000)
//        _SpecColor("Specular Color", Color) = (1.000000,1.000000,1.000000,1.000000)
//        _SpecularArea("Specular Area", Range(0.000000,0.990000)) = 0.100000
//        _SpecularIntensity("Specular Intensity", Range(0.000000,1.000000)) = 0.750000
//        _ReflectionColor("Reflection Tint Color", Color) = (1.000000,1.000000,1.000000,1.000000)
//        _LeftEyeColor("Left Eye Color", COLOR) = (0,1,0,1)
//        _RightEyeColor("Right Eye Color", COLOR) = (1,0,0,1)
//    }
//
//
//        SubShader{
//
//            LOD 300
//            Tags { "RenderType" = "Opaque" }
//
//            Pass {
//
//                Tags { "RenderType" = "Opaque" }
//
//                CGPROGRAM
//                #pragma vertex vert
//                #pragma fragment frag
//                #pragma target 2.0
//
//                float4 _LeftEyeColor;
//                float4 _RightEyeColor;
//                float4x4 _ReflectionMat;
//                float4x4 _WorldMat;
//
//                #include "UnityCG.cginc"
//                #pragma multi_compile_fog
//                #define USING_FOG (defined(FOG_LINEAR) || defined(FOG_EXP) || defined(FOG_EXP2))
//
//                // uniforms
//                // vertex shader input data
//                struct appdata {
//                    float3 pos : POSITION;
//                    half4 color : COLOR;
//                    UNITY_VERTEX_INPUT_INSTANCE_ID
//                };
//
//                // vertex-to-fragment interpolators
//                struct v2f {
//
//                    fixed4 color : COLOR0;
//                    #if USING_FOG
//                        fixed fog : TEXCOORD0;
//                    #endif
//
//                    float4 pos : SV_POSITION;
//
//                    UNITY_VERTEX_INPUT_INSTANCE_ID
//                    UNITY_VERTEX_OUTPUT_STEREO
//                };
//
//                // vertex shader
//                v2f vert(appdata IN) {
//
//                    v2f o;
//
//                    UNITY_SETUP_INSTANCE_ID(IN);
//                    UNITY_INITIALIZE_OUTPUT(v2f, o);
//                    UNITY_INITIALIZE_VERTEX_OUTPUT_STEREO(o);
//
//                    // _ReflectionMat
//                    float4x4 m = UNITY_MATRIX_M;
//                    //if (unity_StereoEyeIndex == 0) {
//                    //    m[3].x += 0.011f;
//                    //}
//                    //else {
//                    //    m[3].x -= 0.011f;
//                    //}
//                    m = mul(m, _ReflectionMat);
//
//
//
//
//                    half4 color = IN.color;
//                    //float3 eyePos = mul(mul(UNITY_MATRIX_M, UNITY_MATRIX_V), float4(IN.pos,1)).xyz;
//                    float3 eyePos = mul(UNITY_MATRIX_MV, float4(IN.pos, 1)).xyz;
//                    half3 viewDir = 0.0;
//                    o.color = saturate(color);
//
//                    // compute texture coordinates
//                    // fog
//                    #if USING_FOG
//                        float fogCoord = length(eyePos.xyz); // radial fog distance
//                        UNITY_CALC_FOG_FACTOR_RAW(fogCoord);
//                        o.fog = saturate(unityFogFactor);
//                    #endif
//
//                        // transform position
//                        o.pos = UnityObjectToClipPos(IN.pos);
//
//                        return o;
//                    }
//
//                // fragment shader
//                fixed4 frag(v2f IN) : SV_Target {
//
//                    UNITY_SETUP_STEREO_EYE_INDEX_POST_VERTEX(IN);
//
//
//                    fixed4 col;
//                    col = IN.color;
//                    // fog
//                    #if USING_FOG
//                    col.rgb = lerp(unity_FogColor.rgb, col.rgb, IN.fog);
//                    #endif
//
//                    return col;
//                }
//
//                ENDCG
//            }
//        }
//            Fallback "Reflective/Specular"
//}