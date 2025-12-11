Texture2D tex : register(t0);
SamplerState samplerLinear : register(s0);
struct Light
{
    float3 Strength;
    
    float3 Direction;   // directional/spot light only
    
};

struct Material
{
    float4 DiffuseAlbedo;//物体基础颜色（可乘纹理）
    float3 FresnelR0;//用于计算高光反射率
    float Shininess;//高光粗糙度参数（0–1），内部乘 256
};


cbuffer staticMeshBuffer : register(b0)
{
    float4x4 W;
    float4x4 VP;
    
};
cbuffer staticLightBuffer : register(b1)
{
     float3 gEyePosW;//camera position
     float4 gDiffuseAlbedo;//material color
     float4 gAmbientLight;
      float  gRoughness;//材质粗糙度
          float3 gFresnelR0;// Fresnel 反射参数
           float3 Strength;
    float3 Direction; 
}

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

struct PS_INPUT
{   float3 PosW : POSITION;
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.PosW=input.Pos;
    output.Pos = mul(input.Pos, W);
    output.Pos = mul(output.Pos, VP);
    output.Normal = mul(input.Normal, W);
    output.Tangent = mul(input.Tangent, W);
    output.TexCoords = input.TexCoords;
    return output;
}
float4 PS(PS_INPUT input) : SV_Target0
{
  
  
    float4 colour = tex.Sample(samplerLinear, input.TexCoords);
    if(colour.a<0.5)
       discard;
    return float4(colour.rgb, 1.0);
}