#include "00_Global.fx"
#include "00_Light.fx"

//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
float3 LightDirection;
Texture2DArray TransformsMap;

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

//-----------------------------------------------------------------------------
//CBuffers
//-----------------------------------------------------------------------------
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

struct TweeFrame
{
	float TakeTime;
	float TweenTime;
	float ChangeTime;
	float Padding;

	AnimationFrame Curr;
	AnimationFrame Next;
};

cbuffer CB_AnimationFrame
{
	TweeFrame TweeFrames;
};

//-----------------------------------------------------------------------------
//Tweening
//-----------------------------------------------------------------------------
void SetAnimationWorld(inout matrix world, VertexModel input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
	//[0] CurrClip, [1] NextClip
	int clip[2];
	int currFrame[2], nextFrame[2];
	float time[2];
	
	//CurrentClip
	clip[0] = TweeFrames.Curr.Clip;
	currFrame[0] = TweeFrames.Curr.CurrFrame;
	nextFrame[0] = TweeFrames.Curr.NextFrame;
	time[0] = TweeFrames.Curr.Time;
	
	//NextClip
	clip[1] = TweeFrames.Next.Clip;
	currFrame[1] = TweeFrames.Next.CurrFrame;
	nextFrame[1] = TweeFrames.Next.NextFrame;
	time[1] = TweeFrames.Next.Time;
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim = 0, nextAnim = 0;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		// -> CurrentClip
		//CurrentFrame Bone Sampling
		c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[0], clip[0], 0)); //_11_12_13_14
		c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[0], clip[0], 0)); //_21_22_23_24
		c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[0], clip[0], 0)); //_31_32_33_34
		c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[0], clip[0], 0)); //_41_42_43_44
		curr = matrix(c0, c1, c2, c3);
		
		//NextFrame Bone Sampling
		n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0)); //_11_12_13_14
		n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0)); //_21_22_23_24
		n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0)); //_31_32_33_34
		n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0)); //_41_42_43_44
		next = matrix(n0, n1, n2, n3);
		
		//Interpolate CurrentFrame ~ NextFrame
		currAnim = lerp(curr, next, time[0]);
		
		// -> NextClip
		if (clip[1] > -1)
		{
			
			//CurrentFrame Bone Sampling
			c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currFrame[1], clip[1], 0)); //_11_12_13_14
			c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currFrame[1], clip[1], 0)); //_21_22_23_24
			c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currFrame[1], clip[1], 0)); //_31_32_33_34
			c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currFrame[1], clip[1], 0)); //_41_42_43_44
			curr = matrix(c0, c1, c2, c3);
		
			//NextFrame Bone Sampling
			n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0)); //_11_12_13_14
			n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0)); //_21_22_23_24
			n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0)); //_31_32_33_34
			n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0)); //_41_42_43_44
			next = matrix(n0, n1, n2, n3);
		
			//Interpolate CurrentFrame ~ NextFrame
			nextAnim = lerp(curr, next, time[0]);
			
			//Tweening
			currAnim = lerp(currAnim, nextAnim, TweeFrames.TweenTime);
		}

		//Skinning
		transform += mul(weights[i], currAnim);
	}

	world = mul(transform, world);
	//Texture(inv * anim * parent) * transform(weigths) * root
}

//-----------------------------------------------------------------------------
//Blending
//-----------------------------------------------------------------------------
struct BlendingFrame
{
	uint Mode;
	float Alpha;
	float2 Padding;

	AnimationFrame Clip[3];
};

cbuffer CB_BlendingFrame
{
	BlendingFrame BlendingFrames;
};

void SetBlendingWorld(inout matrix world, VertexModel input)
{
	float indices[4] = { input.BlendIndices.x, input.BlendIndices.y, input.BlendIndices.z, input.BlendIndices.w };
	float weights[4] = { input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, input.BlendWeights.w };
	
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	
	matrix transform = 0;
	matrix curr = 0, next = 0;
	matrix currAnim[3];
	matrix anim = 0;
	
	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		[unroll(3)]
		for (int c = 0; c < 3; c++)
		{
			//CurrentFrame Bone Sampling
			c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendingFrames.Clip[c].CurrFrame, BlendingFrames.Clip[c].Clip, 0)); //_11_12_13_14
			c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendingFrames.Clip[c].CurrFrame, BlendingFrames.Clip[c].Clip, 0)); //_21_22_23_24
			c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendingFrames.Clip[c].CurrFrame, BlendingFrames.Clip[c].Clip, 0)); //_31_32_33_34
			c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendingFrames.Clip[c].CurrFrame, BlendingFrames.Clip[c].Clip, 0)); //_41_42_43_44
			curr = matrix(c0, c1, c2, c3);
		
			//NextFrame Bone Sampling
			n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, BlendingFrames.Clip[c].NextFrame, BlendingFrames.Clip[c].Clip, 0)); //_11_12_13_14
			n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, BlendingFrames.Clip[c].NextFrame, BlendingFrames.Clip[c].Clip, 0)); //_21_22_23_24
			n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, BlendingFrames.Clip[c].NextFrame, BlendingFrames.Clip[c].Clip, 0)); //_31_32_33_34
			n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, BlendingFrames.Clip[c].NextFrame, BlendingFrames.Clip[c].Clip, 0)); //_41_42_43_44
			next = matrix(n0, n1, n2, n3);
		
			//Interpolate CurrentFrame ~ NextFrame
			currAnim[c] = lerp(curr, next, BlendingFrames.Clip[c].Time);
		}

		//Blending
		float alpha = BlendingFrames.Alpha;
		int clipIndex[2] = { 0, 1 };
		
		if (alpha > 1)
		{
			clipIndex[0] = 1;
			clipIndex[1] = 2;

			alpha -= 1.f;
		}
		
		anim = lerp(currAnim[clipIndex[0]], currAnim[clipIndex[1]], alpha);
		
		//Skinning
		transform += mul(weights[i], anim);
	}

	world = mul(transform, world);
}

//-----------------------------------------------------------------------------
//Shader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexModel input)
{
	VertexOutput output;
    
	if (BlendingFrames.Mode == 0)
		SetAnimationWorld(World, input);
	else
		SetBlendingWorld(World, input);
	
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