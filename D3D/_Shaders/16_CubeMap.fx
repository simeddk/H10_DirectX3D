#include "00_Global.fx"

//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
TextureCube CubeMap;

//-----------------------------------------------------------------------------
//Render
//-----------------------------------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	float3 oPosition : Position1;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.oPosition = input.Position.xyz;
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}


float4 PS_NDC(VertexOutput input) : SV_Target
{
	return CubeMap.Sample(LinearSampler, input.oPosition);
}

float4 PS_Direction(VertexOutput input) : SV_Target
{
	input.oPosition = normalize(input.oPosition - float3(0, 0, 0));
	
	return CubeMap.Sample(LinearSampler, input.oPosition);
}

float4 PS_WireFrame(VertexOutput input) : SV_Target
{
	return float4(1, 0, 0, 1);
}

//-----------------------------------------------------------------------------
//Pipelines
//-----------------------------------------------------------------------------
technique11 T0
{
	P_VP(P0, VS, PS_NDC)
	P_VP(P1, VS, PS_Direction)
	P_RS_VP(P2, FillMode_WireFrame, VS, PS_Direction)
}