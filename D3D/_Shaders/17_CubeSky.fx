#include "00_Global.fx"

//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
TextureCube SkyMap;

//-----------------------------------------------------------------------------
//Render
//-----------------------------------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 oPosition : Position1;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.oPosition = input.Position.xyz;
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	return output;
}


float4 PS_NDC(VertexOutput input) : SV_Target
{
	return SkyMap.Sample(LinearSampler, input.oPosition);
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
	P_RS_DSS_VP(P0, FrontCounterClockwise_True, DepthEnable_False, VS, PS_NDC)
}