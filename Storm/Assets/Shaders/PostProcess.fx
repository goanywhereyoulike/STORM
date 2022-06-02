// Description: Shader for post processing
cbuffer PostProcessBuffer : register(b0)
{
    int mode;
	int textureDimensions;
	int useBlurEffect;
	int useEmbossEffect;
    int useGlitchEffect;
	float embossamount;
    float time;
    float block_size;
    float speed;
}
Texture2D textureMap : register(t0);
Texture2D bloomeMap : register(t1);
SamplerState textureSampler : register(s0);



struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

//float randomNoise(float2 c)
//{
//    return frac(sin(dot(c.xy, float2(12.9898, 78.233))) * 43758.5433);

//}
float Rand(float2 n)
{
    return sin(dot(n, half2(1233.224, 1743.335)));
}
float randomNoise(float seed)
{
    return Rand(float2(seed, 1.0));
}
float randomBlockNoise(float2 seed)
{
    return frac(sin(dot(seed * floor(time * speed), float2(17.13, 3.71))) * 43758.5453123);

}

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = textureMap.Sample(textureSampler, input.texCoord);

    if (mode == 2) // Bloom
    {
        float4 bloom = bloomeMap.Sample(textureSampler, input.texCoord);
        color = color + bloom;
    }
    else
    {
        if (useEmbossEffect)
        {
            color = float4(0.5, 0.5, 0.5, 1.0);
            color -= textureMap.Sample(textureSampler, input.texCoord - 0.003) * embossamount;
            color += textureMap.Sample(textureSampler, input.texCoord + 0.003) * embossamount;
            color.xyz = (color.x + color.y + color.z) / 3.0f;
		
            float4 bloom = bloomeMap.Sample(textureSampler, input.texCoord);
            color = color + bloom;
        }
        if (useBlurEffect)
        {
            float KernelOffsets[3] = { 0.0f, 1.3846153846f, 3.2307692308f };
            float BlurWeights[3] = { 0.2270270270f, 0.3162162162f, 0.0702702703f };
            float3 textureColor = float3(1.0f, 0.0f, 0.0f);
            float2 uv = input.texCoord;

            textureColor = color.xyz * BlurWeights[0];
            for (int i = 1; i < 3; i++)
            {
                float2 normalizedOffset = float2(0.0f, KernelOffsets[i]) / textureDimensions;
                textureColor += textureMap.Sample(textureSampler, uv + normalizedOffset).xyz * BlurWeights[i];
                textureColor += textureMap.Sample(textureSampler, uv - normalizedOffset).xyz * BlurWeights[i];
            }

            for (int j = 1; j < 3; j++)
            {
                float2 normalizedOffset = float2(KernelOffsets[j], 0.0f) / textureDimensions;
                textureColor += textureMap.Sample(textureSampler, uv + normalizedOffset).xyz * BlurWeights[j];
                textureColor += textureMap.Sample(textureSampler, uv - normalizedOffset).xyz * BlurWeights[j];
            }
	
            textureColor = textureColor / 2;
            color = float4(textureColor, 1.0);
            float4 bloom = bloomeMap.Sample(textureSampler, input.texCoord);
            color = color + bloom;
        }
        if (useGlitchEffect)
        {
            //half4 noiseColor = color;

            //half luminance = dot(noiseColor.rgb, float3(0.22, 0.707, 0.071));
            //if (randomNoise(float2(time * 5, time * 5)) > 10)
            //{
            //    noiseColor = float4(luminance, luminance, luminance, luminance);
            //}

            //float noiseX = randomNoise(time * 5 + input.texCoord / float2(-213, 5.53));
            //float noiseY = randomNoise(time * 5 - input.texCoord / float2(213, -5.53));
            //float noiseZ = randomNoise(time * 5 + input.texCoord / float2(213, 5.53));

            //noiseColor.rgb += 0.25 * float3(noiseX, noiseY, noiseZ) - 0.125;

            //noiseColor = lerp(color, noiseColor, 5);
            //color = noiseColor;
            float2 block = randomBlockNoise(floor(input.texCoord * block_size));
            float displaceNoise = pow(block.x, 8.0) * pow(block.x, 3.0);

            half ColorR = textureMap.Sample(textureSampler, input.texCoord).r;
            half ColorG = textureMap.Sample(textureSampler, input.texCoord + float2(displaceNoise * 0.05 * randomNoise(7.0), 0.0)).g;
            half ColorB = textureMap.Sample(textureSampler, input.texCoord - float2(displaceNoise * 0.05 * randomNoise(13.0), 0.0)).b;

            color = half4(ColorR, ColorG, ColorB, 1.0);
            
            
            
        }
        
        
    }
    return color;
}
