// Description: Terrain shader for Storm.

cbuffer ConstantBuffer : register(b0)
{
    matrix world;
    matrix wvp[2];
    float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
    float3 lightDirection;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float materialPower;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useShadowMap;
    float depthBias;
}
//--------------------------------
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D shadowMap : register(t2);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;

};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float4 worldPosition : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    float3 position = input.position;
	
    matrix toNDC = wvp[0];
    matrix toLightNDC = wvp[1];
    matrix toWorld = world;

    output.position = mul(float4(position, 1.0f), toNDC);
    output.worldNormal = mul(input.normal, (float3x3) toWorld);
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(position, 1.0f), toWorld).xyz);
    output.texCoord = input.texCoord;
    output.lightNDCPosition = mul(float4(position, 1.0f), toLightNDC);
    output.worldPosition = mul(float4(position, 1.0f), toWorld);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 L = normalize(input.dirToLight);
    float3 v = normalize(input.dirToView);
    float4 ambient = lightAmbient * materialAmbient;
    float d = saturate(dot(L, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

    float3 r = reflect(-L, n);
    float base = saturate(dot(r, v));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;

    float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 specularMapColor = specularMap.Sample(textureSampler, input.texCoord);
    float4 colorToUse = diffuseMapColor;
    
    if (input.worldPosition.y > 15.0f)
    {
        colorToUse = specularMapColor;
    }
    else if (input.worldPosition.y > 10.0f)
    {
        float t = (input.worldPosition.y - 10.0f) / 5.0f;
        colorToUse = lerp(diffuseMapColor,specularMapColor,t);
    }
    float4 color = (ambient + diffuse) * colorToUse + specular;
	
    if (useShadowMap)
    {
        float actualDepth = 1.0f - input.lightNDCPosition.z / input.lightNDCPosition.w;
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = 1.0f - ((shadowUV.y + 1.0f) * 0.5f);
		
        if (saturate(u) == u && saturate(v) == v)
        {
            float savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.x;
            if (actualDepth + depthBias < savedDepth)
            {
                color = ambient * colorToUse;
            }
        }


    }
	
    return color;

}