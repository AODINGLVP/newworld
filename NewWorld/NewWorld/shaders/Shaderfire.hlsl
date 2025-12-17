

cbuffer staticMeshBuffer 
{
    float4x4 W;
    float4x4 VP;
};



cbuffer TimeBuffer 
{
    float3 fireposition;
    float iTime;
    float3 padTime;
};

struct VS_INPUT
{
    float4 Pos       : POSITION;
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float2 TexCoords : TEXCOORD;
};

struct PS_INPUT
{
    float4 PosH      : SV_POSITION;
    float3 PosW      : POSITION;
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float2 TexCoords : TEXCOORD;
};


float hash1(float n)
{
    return frac(sin(n) * 43758.5453);
}


float3 FireworkParticles(
    float3 worldPos,
    float3 fireworkPos,
    float time,
    float3 baseColor
)
{
    float3 color = float3(0, 0, 0);

    const int NUM_PARTICLES = 109;

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        float angle = hash1(i * 13.37) * 6.2831853;
        float speed = lerp(3.0, 6.0, hash1(i * 91.17));

        float3 dir = normalize(float3(
            cos(angle),
            hash1(i * 7.1) * 1.2,
            sin(angle)
        ));

        float3 p = fireworkPos + dir * speed * time;

        float dist = length(worldPos - p);
        float size = 0.15;
        float fade = saturate(1.0 - time);

        float intensity = smoothstep(size, 0.0, dist) * fade;
        color += baseColor * intensity;
    }

    return color;
}


PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT o;

    float4 posW = mul(input.Pos, W);
    o.PosW = posW.xyz;
    o.PosH = mul(posW, VP);

    float3x3 W3 = (float3x3)W;
    o.Normal  = normalize(mul(input.Normal,  W3));
    o.Tangent = normalize(mul(input.Tangent, W3));

    o.TexCoords = input.TexCoords;

    return o;
}


float4 PS(PS_INPUT input) : SV_Target
{
   
    float3 fireworkPos   = fireposition; // 世界空间位置
    float fireworkTime   = frac(iTime);             
    float3 fireworkColor = float3(1.0f, 0.6f, 0.2f)*10; // 橙黄色

    float3 firework = FireworkParticles(
        input.PosW,
        fireworkPos,
        fireworkTime,
        fireworkColor
    );




    float intensity = length(firework); // 亮度
    
    if (intensity < 0.1f)
        discard;
    float luminance = dot(fireworkColor, float3(0.2126, 0.7152, 0.0722));
    if (luminance < 0.05)
    discard;
    return float4(firework, 1.0f);
}
