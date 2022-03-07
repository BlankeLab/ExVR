Shader "Custom/DoubleSided" {
    Properties{
        _Color("Color", Color) = (1,1,1,1.0)
        _MainTex("Albedo (RGB)", 2D) = "white" {}
    }
        SubShader{
        Tags{ "RenderType" = "Opaque" }
        LOD 200

            cull off

        CGPROGRAM
        // Physically based Standard lighting model, and enable shadows on all light types
        #pragma surface surf StandardSpecular vertex:vert addshadow nolightmap nodirlightmap fullforwardshadows

        #pragma target 3.5

        sampler2D _MainTex;

        struct Input {
            float2 uv_MainTex;
            float vface : VFACE;
        };
        struct appdata {
            float4 vertex : POSITION;
            float4 tangent : TANGENT;
            float3 normal : NORMAL;
            float4 texcoord : TEXCOORD0;
            float4 texcoord1 : TEXCOORD2;
            float4 texcoord2 : TEXCOORD1;
            float4 color : COLOR;
        };

        half _Cutoff;
        half3 _Color;

        void vert(inout appdata v) {
        }


        void surf(Input IN, inout SurfaceOutputStandardSpecular o) {
            half4 tex = tex2D(_MainTex, IN.uv_MainTex.xy);
            o.Albedo = (tex.rgb * _Color);
            o.Normal *= IN.vface;
        }
        ENDCG

    }
        FallBack off
}
