//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
matrix World, View, Projection;
TextureCube SkyMap;

RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

RasterizerState FrontCounterClockwise_True
{
	FrontCounterClockwise = true;
};

DepthStencilState DepthEnable_False
{
	DepthEnable = false;
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
	
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	
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
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_NDC()));
	}

	pass P1
	{
		SetRasterizerState(FrontCounterClockwise_True);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_NDC()));
	}

	pass P2
	{
		SetRasterizerState(FrontCounterClockwise_True);
		SetDepthStencilState(DepthEnable_False, 1);

		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS_NDC()));
	}

}