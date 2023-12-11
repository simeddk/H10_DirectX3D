matrix World;
matrix View;
matrix Projection;

Texture2D DiffuseMap;

SamplerState WrapSampler
{
	AddressU = Wrap;
	AddressV = Wrap;
};

SamplerState MirrorSampler
{
	AddressU = Mirror;
	AddressV = Mirror;
};

SamplerState ClampSampler
{
	AddressU = Clamp;
	AddressV = Clamp;
};

SamplerState BorderSampler
{
	AddressU = Border;
	AddressV = Border;

	BorderColor = float4(0, 1, 0, 1);
};

uint Mode = 0;

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

float4 PS(VertexOutput input) : SV_Target
{
	if (Mode == 0)
		return DiffuseMap.Sample(WrapSampler, input.Uv);
	
	if (Mode == 1)
		return DiffuseMap.Sample(MirrorSampler, input.Uv);
	
	if (Mode == 2)
		return DiffuseMap.Sample(ClampSampler, input.Uv);
	
	if (Mode == 3)
		return DiffuseMap.Sample(BorderSampler, input.Uv);
	
	return DiffuseMap.Sample(WrapSampler, input.Uv);
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}