Shader "Unlit/scene_scaner_mesh_unlit"
{
	SubShader
	{
		Pass
		{
			LOD 200
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			#include "UnityCG.cginc"

			struct appdata
			{
				float4 vertex : POSITION;
				float4 color: COLOR;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				float4 col : COLOR;
			};
			
			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos(v.vertex);
				o.col = v.color;

				return o;
			}
			
			fixed4 frag (v2f o) : COLOR
			{

				//return pow(o.col, 0.4545);
				return pow(o.col, 2.2);
			}
			ENDCG
		}
	}
}
