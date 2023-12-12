//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
matrix World;
matrix View;
matrix Projection;

RasterizerState FillMode_WireFrame
{
	FillMode = WIREFRAME;
};

//-----------------------------------------------------------------------------
//Simple Color Pipeline
//-----------------------------------------------------------------------------
struct VertexInput
{
	float4 Position : Position;
};

struct VertexOutput
{
	float4 Position : SV_Position;
};

VertexOutput VS(VertexInput input)
{
	VertexOutput output;
    
	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);
	
	return output;
}

float4 PS(VertexOutput input) : SV_Target
{
	return float4(0, 1, 0, 1);
}

//-----------------------------------------------------------------------------
//Height Color Pipeline
//-----------------------------------------------------------------------------
struct VertexOutput_HeightColor
{
	float4 Position : SV_Position;
	float4 Color : Color;
	float3 wPosition : Position1;
};

float4 GetHeightColor(float vertexHegiht)
{
	if (vertexHegiht > 20)
		return float4(1, 1, 1, 1);
	
	if (vertexHegiht > 15)
		return float4(0.75, 0.75, 0.75, 1);

	if (vertexHegiht > 10)
		return float4(0, 0.5, 0.5, 1);
	
	if (vertexHegiht > 5)
		return float4(0.5, 0.25, 0.15, 1);
		
	return float4(0, 0.25, 0.1, 1);

}


VertexOutput_HeightColor VS_HeightColor(VertexInput input)
{
	VertexOutput_HeightColor output;
    
	output.Position = mul(input.Position, World); //-> WS
	output.Color = GetHeightColor(output.Position.y);
	output.wPosition = output.Position.xyz;
	output.Position = mul(output.Position, View); //-> ViewSpace
	output.Position = mul(output.Position, Projection); //-> ViewSpace
	
		
	return output;
}

float4 PS_HeightColor_PerVertex(VertexOutput_HeightColor input) : SV_Target
{
	return input.Color; //SS
}

float4 PS_HeightColor_PerPixel(VertexOutput_HeightColor input) : SV_Target
{
	return GetHeightColor(input.wPosition.y); //SS
}

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
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}

	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, VS_HeightColor()));
		SetPixelShader(CompileShader(ps_5_0, PS_HeightColor_PerVertex()));
	}

	pass P3
	{
		SetVertexShader(CompileShader(vs_5_0, VS_HeightColor()));
		SetPixelShader(CompileShader(ps_5_0, PS_HeightColor_PerPixel()));
	}
}