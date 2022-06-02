// Description: Simple shader that does transform.

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
    float4 excolor;
    float size;
}

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

VS_OUTPUT VS(float3 position : POSITION, float4 color : COLOR)
{
	VS_OUTPUT output;
    output.position = mul(float4(position, 1), wvp);
    output.color = color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return excolor ;
}