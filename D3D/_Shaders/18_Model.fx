#include "00_Global.fx"

//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
float3 LightDirection;


//-----------------------------------------------------------------------------
//Render
//-----------------------------------------------------------------------------
struct VertexModel
{
	float4 Position : Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
	float3 Tangent : Tangent;
	float4 BlendIndices : BlendIndices;
	float4 BlendWeights : BlendWeights;
};

#define MAX_MODEL_BONES 250
cbuffer CB_Bones
{
	Matrix BoneTrasnforms[MAX_MODEL_BONES]; //All Bone Matrices

	uint BoneIndex; //This Mesh Sholud Be Attached Bone Index
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

VertexOutput VS(VertexModel input)
{
	VertexOutput output;
    
	World = mul(BoneTrasnforms[BoneIndex], World);
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}


float4 PS(VertexOutput input) : SV_Target
{
	float4 diffuse = float4(1, 1, 1, 1);
	
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