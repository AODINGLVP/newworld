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
    float4 Pos      : POSITION;
    float3 Normal   : NORMAL;
    float3 Tangent  : TANGENT;
    float2 TexCoords: TEXCOORD;
};

struct PS_INPUT
{
    float4 PosH     : SV_POSITION;
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
    PS_INPUT o;

    float4 posW = mul(input.Pos, W);
    o.PosW  = posW.xyz;
    o.PosH  = mul(posW, VP);

    float3x3 W3 = (float3x3)W;
    o.Normal  = normalize(mul(input.Normal,  W3));
    o.Tangent = normalize(mul(input.Tangent, W3));

    o.TexCoords = input.TexCoords;

    return o;
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
