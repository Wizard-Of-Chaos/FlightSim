float4x4 worldViewProj; //World * View * Projection
float4x4 worldInvTranspose;

float3 diffuseDir = float3(1, 0, 0);

float4 lightColor = float4(1, 1, 1, 1);
float4 lightIntensity = .4;

struct VS_OUTPUT
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
};

VS_OUTPUT vertexMain(
    in float4 vPosition : POSITION0,
    in float3 vNormal : NORMAL0
)
{
    VS_OUTPUT output;
    output.Position = mul(vPosition, worldViewProj);
    
    float4 norm = mul(vNormal, WorldInvTranspose);
    float intensity = dot(norm, diffuseDir);
    output.Color = saturate(lightColor * lightIntensity * intensity);
    return output;
}

struct PS_OUTPUT
{
    float4 Color : COLOR0;
};

PS_OUTPUT pixelMain(float4 color : COLOR0)
{
    PS_OUTPUT output;
    output.Color = saturate(color + lightColor * lightIntensity);
    return output; 
}

