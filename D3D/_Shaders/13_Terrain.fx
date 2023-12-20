#include "00_Global.fx"

//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
Texture2D BaseMap;
Texture2D LayerMap;
Texture2D AlphaMap;
float3 LightDirection;
float Tile;
float AlphaIntensity;

//-----------------------------------------------------------------------------
//Render
//-----------------------------------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}


float4 PS(VertexOutput input) : SV_Target
{
	float4 base = BaseMap.Sample(LinearSampler, input.Uv * Tile);
	float4 layer = LayerMap.Sample(LinearSampler, input.Uv * Tile);
	float4 alpha = AlphaMap.Sample(LinearSampler, input.Uv);
	
	float4 diffuse = lerp(base, layer, saturate(alpha.r * AlphaIntensity));
	
	float3 normal = normalize(input.Normal);
	float lambert = saturate(dot(normal, -LightDirection));
	
	return diffuse * lambert;
}

float4 PS_WireFrame(VertexOutput input) : SV_Target
{
	return float4(0.75, 0.75, 0.75, 1);
}

//-----------------------------------------------------------------------------
//Pipelines
//-----------------------------------------------------------------------------
technique11 T0
{
	P_VP(P0, VS, PS)
	P_RS_VP(P1, FillMode_WireFrame, VS, PS_WireFrame)
}