cbuffer CameraCB : register(b0)
{
    float4x4 View;
    float4x4 Projection;
};

struct VS_IN
{
    float3 Pos : POSITION;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Dir : TEXCOORD0;
};

VS_OUT VS(VS_IN input)
{
    VS_OUT o;

    float4x4 viewNoTranslation = View;
    viewNoTranslation._41 = 0;
    viewNoTranslation._42 = 0;
    viewNoTranslation._43 = 0;

    o.Pos = mul(Projection,
                mul(viewNoTranslation, float4(input.Pos, 1.0)));

    // 方向用于 cubemap 采样
    o.Dir = input.Pos;
    return o;
}
