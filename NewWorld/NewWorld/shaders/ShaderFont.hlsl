Texture2D tex : register(t0);
SamplerState samplerLinear : register(s0);



struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    
   
    output.Pos = float4((input.Pos.x-960)/960, (input.Pos.y-540)/540, 0.0f, 1.0f);

  
    output.Normal  = input.Normal;
    output.Tangent = input.Tangent;

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