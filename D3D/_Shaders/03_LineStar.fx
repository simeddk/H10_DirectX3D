struct VertexInput
{
	float4 Position : Position;
	float4 Color : Color;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float4 Color : Color;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.Position = input.Position;
	output.Color = input.Color;

	return output;
}

float4 PS_VertexColor(VertexOutput input) : SV_Target
{
	return input.Color;
}

float4 StarColor;
float4 PS_StarColor(VertexOutput input) : SV_Target
{
	return StarColor;
}

technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_VertexColor()));
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_StarColor()));
	}
}