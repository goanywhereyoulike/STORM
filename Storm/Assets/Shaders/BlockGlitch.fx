// Description: Shader for post processing
cbuffer BlockGlitchBuffer : register(b0)
{
    int mode;
    int useGlitchEffect;
    float time;
    float block_size;
    float speed;
    float fade;
    float offset;
    float _BlockLayer1_U;
    float _BlockLayer1_V;
    float _BlockLayer2_U;
    float _BlockLayer2_V;
    float _RGBSplit_Indensity;
    float _BlockLayer1_Indensity;
    float _BlockLayer2_Indensity;
    
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

float randomNoise(float2 seed)
{
    return frac(sin(dot(seed * floor(time * speed), float2(127.1, 311.7))) * 43758.5453123);
}
float randomNoise(float seed)
{
    return randomNoise(float2(seed, 1.0));
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
        if (useGlitchEffect)
        {

            float2 uv = input.texCoord.xy;
		
            float2 blockLayer1 = floor(uv * float2(_BlockLayer1_U, _BlockLayer1_V));
            float2 blockLayer2 = floor(uv * float2(_BlockLayer2_U, _BlockLayer2_V));
            
            float lineNoise1 = pow(randomNoise(blockLayer1), _BlockLayer1_Indensity);
            float lineNoise2 = pow(randomNoise(blockLayer2), _BlockLayer2_Indensity);
            float RGBSplitNoise = pow(randomNoise(5.1379), 7.1) * _RGBSplit_Indensity;
            float lineNoise = lineNoise1 * lineNoise2 * offset - RGBSplitNoise;
		
            float4 colorR = textureMap.Sample(textureSampler, uv);
            float4 colorG = textureMap.Sample(textureSampler, uv + float2(lineNoise * 0.05 * randomNoise(7.0), 0));
            float4 colorB = textureMap.Sample(textureSampler, uv - float2(lineNoise * 0.05 * randomNoise(23.0), 0));
		
            float4 result = float4(float3(colorR.x, colorG.y, colorB.z), colorR.a + colorG.a + colorB.a);
            result = lerp(colorR, result, fade);
		
            color = result;
            
            
            
        }
        
        
    }
    return color;
}
