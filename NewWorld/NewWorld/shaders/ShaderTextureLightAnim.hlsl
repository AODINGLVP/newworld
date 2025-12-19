Texture2D tex : register(t0);
Texture2D NHtex : register(t1);
SamplerState samplerLinear : register(s0);

struct Light
{
    float3 Strength;
    float3 Direction;
};



cbuffer staticMeshBuffer 
{
    float4x4 W;
    float4x4 VP;
    float4x4 bones[256];
};

cbuffer staticLightBuffer 
{
  
    float  pad0;            
   
    float4 gAmbientLight;
   
    float3 Strength;
    float3 Direction;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    uint4 BoneIDs : BONEIDS;
    float4 BoneWeights : BONEWEIGHTS;
};

struct PS_INPUT
{
    float4 Pos     : SV_POSITION;
    float3 PosW     : POSITION;
    float3 Normal   : NORMAL;
    float3 Tangent  : TANGENT;
    float2 TexCoords: TEXCOORD;
};





float3 ComputeDirectionalLight(Light L, float3 normal )
{
    float3 lightVec = -normalize(L.Direction);//calculate the vector from surface to light
    float ndotl = max(dot(lightVec, normal), 0.0f);//Lambert’s Cosine Law,calculate the strength of light 
    float3 lightStrength = L.Strength * ndotl;//mix

    return lightStrength;
}

float4 ComputeLighting(Light L,float3 pos, float3 normal )
{
    float3 result =  ComputeDirectionalLight(L, normal);
    return float4(result, 0.0f);
}

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
float4 pos = input.Pos;
float4x4 transform;
transform = bones[input.BoneIDs[0]] * input.BoneWeights[0];
transform += bones[input.BoneIDs[1]] * input.BoneWeights[1];
transform += bones[input.BoneIDs[2]] * input.BoneWeights[2];
transform += bones[input.BoneIDs[3]] * input.BoneWeights[3];
    output.Pos = mul(pos, transform);
    output.Pos = mul(output.Pos, W);
    output.Pos = mul(output.Pos, VP);
    output.Normal = mul(input.Normal, (float3x3)transform);
    output.Normal = mul(output.Normal, (float3x3)W);
output.Normal = normalize(output.Normal);
    output.Tangent = mul(input.Tangent, (float3x3)transform);
    output.Tangent = mul(output.Tangent, (float3x3)W);
output.Tangent = normalize(output.Tangent);
    output.TexCoords = input.TexCoords;
   float4 posW = mul(pos, transform);
posW = mul(posW, W);
output.PosW = posW.xyz;
    return output;
}

float4 PS(PS_INPUT input) : SV_Target0
{
    
    float4 colour = tex.Sample(samplerLinear, input.TexCoords);
    if (colour.a < 0.5f)
        discard;

   
    float3 normalTS = NHtex.Sample(samplerLinear, input.TexCoords).xyz;
    normalTS = normalize(normalTS * 2.0f - 1.0f);   // turn Vec to normal

   
    float3 N = normalize(input.Normal);
    float3 T = normalize(input.Tangent);
    T = normalize(T - dot(T, N) * N);
    float3 B = cross(N, T);
    float3x3 TBN = float3x3(T, B, N);

   
    float3 normalWS = normalize(mul(normalTS, TBN));//turn to world space normal

    
    

   
    Light L;
    L.Strength  = Strength;
    L.Direction = normalize(Direction);

    float4 ambient = gAmbientLight ;//ambient light

 
   

   
   
    float4 directLight = ComputeLighting(L, input.PosW, normalWS);

    
    float3 lighting = ambient.rgb + directLight.rgb;

  
    float3 finalColor = colour.rgb * lighting;

    return float4(finalColor, colour.a);
}
