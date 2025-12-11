Texture2D tex : register(t0);
SamplerState samplerLinear : register(s0);

struct Light
{
    float3 Strength;
    float3 Direction;
};

struct Material
{
    float4 DiffuseAlbedo;
    float3 FresnelR0;
    float  Shininess;
};

cbuffer staticMeshBuffer 
{
    float4x4 W;
    float4x4 VP;
};

cbuffer staticLightBuffer 
{
    float3 gEyePosW;
    float  pad0;             // 对齐用
    float4 gDiffuseAlbedo;
    float4 gAmbientLight;
    float  gRoughness;
    float3 gFresnelR0;
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

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));
    float f0 = 1.0f - cosIncidentAngle;
    float3 reflectPercent = R0 + (1.0f - R0)*(f0*f0*f0*f0*f0);
    return reflectPercent;
}

float3 BlinnPhong(float3 lightStrength, float3 lightVec, float3 normal, float3 toEye, Material mat)
{
    const float m = mat.Shininess * 256.0f;
    float3 halfVec = normalize(toEye + lightVec);
    float ndoth = max(dot(halfVec, normal), 0.0f);

    float roughnessFactor = (m + 8.0f) * pow(ndoth, m) / 8.0f;
    float3 fresnelFactor = SchlickFresnel(mat.FresnelR0, halfVec, lightVec);

    float3 specAlbedo = fresnelFactor * roughnessFactor;
    specAlbedo = specAlbedo / (specAlbedo + 1.0f);

    return (mat.DiffuseAlbedo.rgb + specAlbedo) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEye)
{
    float3 lightVec = -normalize(L.Direction);
    float ndotl = max(dot(lightVec, normal), 0.0f);
    float3 lightStrength = L.Strength * ndotl;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, mat);
}

float4 ComputeLighting(Light L, Material mat,
                       float3 pos, float3 normal, float3 toEye,
                       float shadowFactor)
{
    float3 result = shadowFactor * ComputeDirectionalLight(L, mat, normal, toEye);
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
    // 采样纹理
    float4 colour = tex.Sample(samplerLinear, input.TexCoords);
    if (colour.a < 0.5f)
        discard;

    // 世界空间法线 / 视线方向
    float3 N      = normalize(input.Normal);
    float3 toEyeW = normalize(gEyePosW - input.PosW);

    // 灯光 & 材质
    Light L;
    L.Strength  = Strength;
    L.Direction = Direction;     // 可再 normalize

    float4 ambient = gAmbientLight * gDiffuseAlbedo;

    float shininess = 1.0f - gRoughness;
    Material mat;
    mat.DiffuseAlbedo = gDiffuseAlbedo;
    mat.FresnelR0     = gFresnelR0;
    mat.Shininess     = shininess;

    float shadowFactor = 1.0f;
    float4 directLight = ComputeLighting(L, mat, input.PosW, N, toEyeW, shadowFactor);

    // 合并光照 & 材质 & 纹理
    
    float3 lighting = ambient.rgb + directLight.rgb;
    float3 finalColor = colour.rgb * lighting;

    return float4(finalColor, colour.a);
}
