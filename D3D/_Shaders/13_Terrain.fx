//-----------------------------------------------------------------------------
//Parmameters
//-----------------------------------------------------------------------------
matrix World, View, Projection;
Texture2D BaseMap;
Texture2D LayerMap;
Texture2D AlphaMap;
float3 LightDirection;
float Tile;
float AlphaIntensity;

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
	float3 Normal : Normal;
	float2 Uv : Uv;
};

struct VertexOutput
{
	float4 Position : SV_Position;
	float3 Normal : Normal;
	float2 Uv : Uv;
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
	float4 base = BaseMap.Sample(LinearSampler, input.Uv * Tile);
	float4 layer = LayerMap.Sample(LinearSampler, input.Uv * Tile);
	float4 alpha = AlphaMap.Sample(LinearSampler, input.Uv/* * Tile*/);
	
	float4 diffuse = lerp(base, layer, saturate(alpha.r * AlphaIntensity));
	
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