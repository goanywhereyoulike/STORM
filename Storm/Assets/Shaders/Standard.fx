// Description: Default Shader for Storm

cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix wvp[2];
    float3 viewPosition;
}
cbuffer BoneTransformBuffer : register(b1)
{
    matrix boneTransforms[128];

}

cbuffer LightBuffer : register(b2)
{
    float3 lightDirection;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b3)
{
    float4 materialAmbient;
    float4 materialDiffuse;
    float4 materialSpecular;
    float materialPower;
}

cbuffer SettingsBuffer : register(b4)
{
    bool useSpecularMap; //4 bytes in HLSL
    bool useDisplacementMap;
    float displacementWeight;
    bool useNormalMap;
    bool useShadowMap;
    bool useSkinning;
    float depthBias;
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D shadowMap : register(t4);
SamplerState textureSampler : register(s0);


struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
    int4 blendIndices : BLENDINDICES;
    float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 worldTangent : TANGENT;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
};

static matrix Identity =
{
    1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
    if (length(weights) <= 0.0f)
        return Identity;

    matrix transform;
    transform = boneTransforms[indices[0]] * weights[0];
    transform += boneTransforms[indices[1]] * weights[1];
    transform += boneTransforms[indices[2]] * weights[2];
    transform += boneTransforms[indices[3]] * weights[3];
    return transform;
}


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    float3 position = input.position;
    if (useDisplacementMap)
    {
        float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).r;
        position = position + (input.normal * displacement * displacementWeight);
    }
	
    
    matrix toNDC = wvp[0];
    matrix toLightNDC = wvp[1];
    matrix toWorld = world;

    if (useSkinning)
    {
        matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
        toNDC = mul(boneTransform, toNDC);
        toLightNDC = mul(boneTransform, toLightNDC);
        toWorld = mul(boneTransform, toWorld);

    }

    output.position = mul(float4(position, 1.0f), toNDC);
    output.worldNormal = mul(input.normal, (float3x3) toWorld);
    output.worldTangent = mul(input.tangent, (float3x3) toWorld);
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - mul(float4(position, 1.0f), toWorld).xyz);
    output.texCoord = input.texCoord;
    output.lightNDCPosition = mul(float4(position, 1.0f), toLightNDC);
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 L = normalize(input.dirToLight);
    float3 v = normalize(input.dirToView);

    if (useNormalMap)
    {
        float3 t = normalize(input.worldTangent);
        float3 b = normalize(cross(n, t));
        float4 normalMapColor = normalMap.Sample(textureSampler, input.texCoord);
        float3 unpackedNormal = normalize(float3((normalMapColor.xy * 2.0f) - 1.0f, normalMapColor.z));
        float3x3 tbnw = float3x3(t, b, n);
        n = mul(unpackedNormal, tbnw);
    }


    float4 ambient = lightAmbient * materialAmbient;
    float d = saturate(dot(L, n));
    float4 diffuse = d * lightDiffuse * materialDiffuse;

    float3 r = reflect(-L, n);
    float base = saturate(dot(r, v));
    float s = pow(base, materialPower);
    float4 specular = s * lightSpecular * materialSpecular;

    float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);
    float4 specularMapColor = 1.0f;
    if (useSpecularMap)
    {
        specularMapColor = specularMap.Sample(textureSampler, input.texCoord).r;
    }
	
    float4 color = (ambient + diffuse) * diffuseMapColor + (specular * specularMapColor);
	
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
                color = ambient * diffuseMapColor;
            }
        }


    }
	
    return color;

}

