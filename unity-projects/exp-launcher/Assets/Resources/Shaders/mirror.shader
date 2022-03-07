Shader "Unlit/mirror"
{
    Properties
    {
        _ReflectionTex ("Texture", 2D) = "white" {}
		[IntRange] _StencilRef("Stencil Reference Value", Range(0,255)) = 0
    }
    SubShader
    {
        Tags { "RenderType"="Transparent" "Queue" = "Transparent+1" }
        LOD 100
		Blend SrcAlpha OneMinusSrcAlpha

        Pass
        {
			Stencil{
				Ref[_StencilRef]
				Comp Equal
			}

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag


            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
                float4 vertex : SV_POSITION;
            };

            sampler2D _ReflectionTex;
			float _reflectionFactor;


            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
				o.uv = v.uv;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_ReflectionTex, i.uv);
				col.a = _reflectionFactor;
				//return fixed4(1, 0, 0, 1);
                return col;
            }
            ENDCG
        }
    }
}
