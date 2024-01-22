#include "00_Global.fx"
#include "00_Light.fx"

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

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
	float3 Normal : Normal;
};

#define MAX_MODEL_BONES 250
#define MAX_MODEL_FRAMES 500

cbuffer CB_Bones
{
	Matrix BoneTrasnforms[MAX_MODEL_BONES]; //All Bone Matrices

	uint BoneIndex; //This Mesh Sholud Be Attached Bone Index
};

struct AnimationFrame
{
	int Clip;

	uint CurrFrame;
	uint NextFrame;

	float Time;
	float RunningTime;

	float Speed;
};

cbuffer CB_KeyFrames
{
	AnimationFrame AnimationFrames;
};

Texture2DArray TransformsMap;

void SetAnimationWorld(inout matrix world, VertexModel input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
	int clip;
	int currFrame;
	
	clip = AnimationFrames.Clip;
	currFrame = AnimationFrames.CurrFrame;
	
	float4 c0, c1, c2, c3;
	
	matrix transform = 0;
	matrix curr = 0;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame, clip, 0)); //_11_12_13_14
		c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame, clip, 0)); //_21_22_23_24
		c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame, clip, 0)); //_31_32_33_34
		c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame, clip, 0)); //_41_42_43_44
		curr = matrix(c0, c1, c2, c3);

		transform += mul(weights[i], curr);
	}

	world = mul(transform, world);
	//Texture(inv * anim * parent) * transform(weigths) * root
}

VertexOutput VS(VertexModel input)
{
	VertexOutput output;
    
	SetAnimationWorld(World, input);
	
	output.Position = WorldPosition(input.Position);
	output.Position = ViewProjection(output.Position);
	
	output.Normal = WorldNormal(input.Normal);
	
	output.Uv = input.Uv;
	
	return output;
}


float4 PS(VertexOutput input) : SV_Target
{
	float4 diffuse = DiffuseMap.Sample(LinearSampler, input.Uv);
	
	float3 normal = normalize(input.Normal);
	float lambert = saturate(dot(normal, -LightDirection));
	
	return diffuse * lambert;
}

float4 PS_WireFrame(VertexOutput input) : SV_Target
{
	return float4(1, 0, 0, 1);
	//return float4(0.75, 0.75, 0.75, 1);
}

//-----------------------------------------------------------------------------
//Pipelines
//-----------------------------------------------------------------------------
technique11 T0
{
	P_VP(P0, VS, PS)
	P_RS_VP(P1, FillMode_WireFrame, VS, PS_WireFrame)
}