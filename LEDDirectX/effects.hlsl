/* Matrices of transformations. */
matrix worldMatrix;
matrix viewMatrix;
matrix projectionMatrix;

float4 diffuseColor;
float3 lightDirection;
float4 ambientColor;

struct VertexInputType
{
	float4 position : POSITION;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.normal = mul(input.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);
	return output;
}

/* Pixel shader. */
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float3 lightDir;
	float lightIntensity;
	float4 color;

	lightDir = -lightDirection;
	lightIntensity = saturate(dot(input.normal, lightDir));

	color = ambientColor;

	if (lightIntensity > 0.0f)
		color += saturate(diffuseColor * lightIntensity);

	color = saturate(color);
	return color;
}

technique10 LightTechnique
{
	pass pass0
	{
		SetVertexShader(CompileShader(vs_4_0, LightVertexShader()));
		SetPixelShader(CompileShader(ps_4_0, LightPixelShader()));
		SetGeometryShader(NULL);
	}
}