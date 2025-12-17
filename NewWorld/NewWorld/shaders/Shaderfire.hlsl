

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
    return frac(sin(n) * 52356.1237);
}


float3 FireworkParticles( float3 worldPos,float3 fireworkPos,float time,float3 baseColor
)
{
    float seed = hash1(dot(fireworkPos, float3(12.9898, 78.233, 45.164)));
    //random seed
    float3 color = float3(0, 0, 0);

    const int NUM_PARTICLES = 109;

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        
        float speed = lerp(3.0, 6.0, hash1(i *seed* 31.35));
        //get random speed form 3 to 6
        float3 dir = normalize(float3(
            hash1(i*seed * 33.95)*2-1 ,
            hash1(i*seed * 7.1)*2-1 ,
            hash1(i*seed * 25.12)*2-1
        ));
    //random direction
        float3 p = fireworkPos + dir * speed * time;
        //now position of particle
        float dist = length(worldPos - p);
        float size = 0.1;
        float fade = saturate(1.0 - time);

        float intensity = smoothstep(size, 0.0, dist) * fade;
        //calculate color based on distance
        //the dis more close to zero more light

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
   
    float3 fireworkPos   = fireposition; 
    float fireworkTime   = frac(iTime);             
    float3 fireworkColor = float3(1.0f, 0.6f, 0.2f)*10; 

    float3 firework = FireworkParticles(
        input.PosW,
        fireworkPos,
        fireworkTime,
        fireworkColor
    );




    float intensity = length(firework); 
    
   if (intensity < 0.1f)//cut the dark part
       discard;
    float luminance = dot(fireworkColor, float3(0.2126, 0.7152, 0.0722));//cut the dark part
    if (luminance < 0.05)
   discard;
    return float4(firework, 1.0f);
}
