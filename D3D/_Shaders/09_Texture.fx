matrix World;
matrix View;
matrix Projection;

Texture2D DiffuseMap;

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VertexInput
{
	float4 Position : Position;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float2 Uv : Uv;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Uv = input.Uv;
	
	return output;
}

float4 PS_PointSampler(VertexOutput input) : SV_Target
{
	return DiffuseMap.Sample(PointSampler, input.Uv);
}

float4 PS_LinearSampler(VertexOutput input) : SV_Target
{
	return DiffuseMap.Sample(LinearSampler, input.Uv);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_PointSampler()));
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_LinearSampler()));
	}
}