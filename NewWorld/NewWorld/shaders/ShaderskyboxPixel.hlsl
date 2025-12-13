// Skybox Pixel Shader
TextureCube skyTex : register(t0);
SamplerState samp  : register(s0);

// ★ 必须和 VS 输出语义一模一样
struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Dir : TEXCOORD0;
};

float4 PS(VS_OUT input) : SV_Target
{
    // cubemap 只接受方向向量
    float3 dir = normalize(input.Dir);

    float4 color = skyTex.Sample(samp, dir);

    return color;
}
