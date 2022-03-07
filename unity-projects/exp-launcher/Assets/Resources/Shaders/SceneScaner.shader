
Shader "Instanced/InstancedShader" {

	SubShader{

		Pass {

			Tags {"LightMode" = "ForwardBase"}
			//Tags { "RenderType" = "Opaque" }

			CGPROGRAM

			#pragma vertex vert
			#pragma fragment frag
			//#pragma multi_compile_fwdbase //nolightmap nodirlightmap nodynlightmap novertexlight
			#pragma target 4.5

			#include "UnityCG.cginc"

			//sampler2D _MainTex;
			StructuredBuffer<float4> positionBuffer;
			StructuredBuffer<float4> colorBuffer;

			struct v2f {
				float4 pos	 : SV_POSITION;
				float4 color : COLOR;
			};
			
			v2f vert(appdata_full v, uint instanceID : SV_InstanceID) {

				float4 data = positionBuffer[instanceID];
				float4 color = colorBuffer[instanceID];


				float3 localPosition = v.vertex.xyz * data.w;
				float3 worldPosition = data.xyz + localPosition;
				float3 worldNormal	 = v.normal;

				v2f o;
				o.pos = mul(UNITY_MATRIX_VP, float4(worldPosition, 1.0f));
				o.color = color;
				return o;
			}

			fixed4 frag(v2f i) : SV_Target
			{
				fixed4 output = i.color*1.0f;
				return output;
			}

			ENDCG
		}
	}
}


//Shader "Instanced/InstancedSurfaceShader" {
//	Properties{
//		_MainTex("Albedo (RGB)", 2D) = "white" {}
//		_Glossiness("Smoothness", Range(0,1)) = 0.5
//		_Metallic("Metallic", Range(0,1)) = 0.0
//	}
//		SubShader{
//			Tags { "RenderType" = "Opaque" }
//			LOD 200
//
//			CGPROGRAM
//			// Physically based Standard lighting model
//			#pragma surface surf Standard addshadow fullforwardshadows
//			#pragma multi_compile_instancing
//			#pragma instancing_options procedural:setup
//
//			sampler2D _MainTex;
//
//			struct Input {
//				float2 uv_MainTex;
//			};
//
//		#ifdef UNITY_PROCEDURAL_INSTANCING_ENABLED
//			StructuredBuffer<float4> positionBuffer;
//		#endif
//
//			void rotate2D(inout float2 v, float r) {
//				float s, c;
//				sincos(r, s, c);
//				v = float2(v.x * c - v.y * s, v.x * s + v.y * c);
//			}
//
//			void setup() {
//			#ifdef UNITY_PROCEDURAL_INSTANCING_ENABLED
//				float4 data = positionBuffer[unity_InstanceID];
//
//				float rotation = data.w * data.w * _Time.y * 0.5f;
//				rotate2D(data.xz, rotation);
//
//				unity_ObjectToWorld._11_21_31_41 = float4(data.w, 0, 0, 0);
//				unity_ObjectToWorld._12_22_32_42 = float4(0, data.w, 0, 0);
//				unity_ObjectToWorld._13_23_33_43 = float4(0, 0, data.w, 0);
//				unity_ObjectToWorld._14_24_34_44 = float4(data.xyz, 1);
//				unity_WorldToObject = unity_ObjectToWorld;
//				unity_WorldToObject._14_24_34 *= -1;
//				unity_WorldToObject._11_22_33 = 1.0f / unity_WorldToObject._11_22_33;
//			#endif
//			}
//
//			half _Glossiness;
//			half _Metallic;
//
//			void surf(Input IN, inout SurfaceOutputStandard o) {
//				fixed4 c = tex2D(_MainTex, IN.uv_MainTex);
//				o.Albedo = c.rgb;
//				o.Metallic = _Metallic;
//				o.Smoothness = _Glossiness;
//				o.Alpha = c.a;
//			}
//			ENDCG
//		}
//			FallBack "Diffuse"
//}



//// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'
//
//
//Shader "Custom/SceneScaner"
//{
//	Properties
//	{
//		// Color property for material inspector, default to white
//		_Color("Main Color", Color) = (1,1,1,1)
//	}
//		SubShader
//	{
//		Pass
//		{
//			CGPROGRAM
//			#pragma vertex vert
//			#pragma fragment frag
//
//			// vertex shader
//			// this time instead of using "appdata" struct, just spell inputs manually,
//			// and instead of returning v2f struct, also just return a single output
//			// float4 clip position
//			float4 vert(float4 vertex : POSITION) : SV_POSITION
//			{
//				return UnityObjectToClipPos(vertex);
//			}
//
//		// color from the material
//		fixed4 _Color;
//
//		// pixel shader, no inputs needed
//		fixed4 frag() : SV_Target
//		{
//			return _Color; // just return it
//		}
//		ENDCG
//	}
//	}
//}
////// Upgrade NOTE: replaced 'mul(UNITY_MATRIX_MVP,*)' with 'UnityObjectToClipPos(*)'
////
////// Shader "name" { [Properties] __Subshaders__ [Fallback] [CustomEditor] }
////Shader "Unlit/SceneScaner"
////{
////	Properties // any properties declared in a shader are shown in the material inspector inside Unity
////	{
////		//_Color("Main Color", Color) = (1,.5,.5,1)
////		[NoScaleOffset] _MainTex("Texture", 2D) = "white" {}
////	}
////	SubShader // Each shader is comprised of a list of sub-shaders. You must have at least one.
////	{
////		//Tags { "RenderType"="Opaque" }
////		//LOD 100
////
////		// Builtin LOD
////		// VertexLit kind of shaders = 100
////		// Decal, Reflective VertexLit = 150
////		// Diffuse = 200
////		// Diffuse Detail, Reflective Bumped Unlit, Reflective Bumped VertexLit = 250
////		// Bumped, Specular = 300
////		// Bumped Specular = 400
////		// Parallax = 500
////		// Parallax Specular = 600
////
////		// a single pass in our subshader
////		Pass
////		{
////			//CGPROGRAM
////			//#pragma vertex vert
////			//#pragma fragment frag
////			//#include "UnityCG.cginc"
////
////			//struct v2f {
////			//	float4 pos : SV_POSITION;
////			//	float4 col : COLOR;
////			//};
////
////			//// use fixed function per-vertex lighting
////
////			//v2f vert(float4 vertex : POSITION, float4 color : COLOR) {
////			//	v2f o;
////			//	o.pos = UnityObjectToClipPos(vertex); // mul(UNITY_MATRIX_MVP, float4(pos, 1.0))
////			//	o.col = color;
////			//	//o.col.xyz = 
////			//	//o.worldPos = mul(_Object2World, vertex).xyz;
////			//	//o.worldNormal = UnityObjectToWorldNormal(normal);
////			//	return o;
////			//}
////
////			//fixed4 frag (v2f i) : SV_Target{
////			//	return i.col;
////			//}
////			//	ENDCG // end HLSL/Cg code
////
////			//CGPROGRAM
////			//#pragma vertex vert
////			//#pragma fragment frag
////			//#include "UnityCG.cginc"
////
////			//// vertex input: position, color
////			//struct appdata {
////			//	float4 vertex : POSITION;
////			//	fixed4 color : COLOR;
////			//};
////
////			//struct v2f {
////			//	float4 pos : SV_POSITION;
////			//	fixed4 color : COLOR;
////			//};
////
////			//v2f vert(appdata v) {
////			//	v2f o;
////			//	o.pos = UnityObjectToClipPos(v.vertex);
////			//	o.color = v.color;
////			//	return o;
////			//}
////
////			//fixed4 frag(v2f i) : SV_Target{ return i.color; }
////			//ENDCG
////
////			CGPROGRAM
////			// use "vert" function as the vertex shader
////			#pragma vertex vert
////			// use "frag" function as the pixel (fragment) shader
////			#pragma fragment frag
////
////			// vertex shader inputs
////			struct appdata {
////				float4 vertex : POSITION; // vertex position
////				float2 uv : TEXCOORD0; // texture coordinate
////			};
////
////			// vertex shader outputs ("vertex to fragment")
////			struct v2f {
////				float2 uv : TEXCOORD0; // texture coordinate
////				float4 vertex : SV_POSITION; // clip space position
////			};
////
////			// vertex shader
////			v2f vert(appdata v) {
////				v2f o;
////				// transform position to clip space
////				// (multiply with model*view*projection matrix)
////				o.vertex = UnityObjectToClipPos(v.vertex);
////				// just pass the texture coordinate
////				o.uv = v.uv;
////				return o;
////			}
////
////			// texture we will sample
////			sampler2D _MainTex;
////
////			// pixel shader; returns low precision ("fixed4" type)
////			// color ("SV_Target" semantic)
////			fixed4 frag(v2f i) : SV_Target
////			{
////				// sample texture and return it
////				fixed4 col = tex2D(_MainTex, i.uv);
////				return col;
////			}
////			ENDCG
////		}
////	}
////
////	//Fallback "otherexample" After all Subshaders a Fallback can be defined. It basically says “if none of subshaders can run on this hardware, try using the ones from another shader
////}
