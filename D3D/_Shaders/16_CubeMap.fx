//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
matrix World, View, Projection;
TextureCube CubeMap;

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
	float3 oPosition : Position1;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.oPosition = input.Position.xyz;
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	output.Normal = mul(input.Normal, (float3x3)World);
	
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
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_NDC()));
	}

	pass P1
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_Direction()));
	}

	pass P2
	{
		SetRasterizerState(FillMode_WireFrame);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_WireFrame()));
	}
}