//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
matrix World, View, Projection;
Texture2D DiffuseMap;
float3 LightDirection;

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

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
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Normal = mul(input.Normal, (float3x3)World);
	
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
	return float4(0.75, 0.75, 0.75, 1);
}

//-----------------------------------------------------------------------------
//Pipelines
//-----------------------------------------------------------------------------
technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P1
	{
		SetRasterizerState(FillMode_WireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_WireFrame()));
	}
}