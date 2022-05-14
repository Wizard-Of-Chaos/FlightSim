float4x4 world;
float4x4 worldViewProj; //World * View * Projection
float4x4 worldInvTranspose;

//diffuse
float3 diffuseDir = float3(1, 0, 0); // this needs to be set elsewhere
float4 lightColor = float4(1, 1, 1, 1);
float4 lightIntensity = .4;

//specular 
float shine = 200;
float4 specColor = float4(1, 1, 1, 1);
float specIntensity = 1;
float3 viewVec = float3(1, 0, 0); //this needs to be set elsewhere

struct VS_OUTPUT
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : TEXCOORD0;
};

VS_OUTPUT vertexMain(
    in float4 vPosition : POSITION0,
    in float3 vNormal : NORMAL0
)
{
    VS_OUTPUT output;
    output.Position = mul(vPosition, worldViewProj);
    
    float4 norm = normalize(mul(vNormal, WorldInvTranspose));
    float intensity = dot(norm, diffuseDir);
    output.Color = saturate(lightColor * lightIntensity * intensity);
    
    output.Normal = norm;
    
    return output;
}

struct PS_OUTPUT
{
    float4 Color : COLOR0;
};

PS_OUTPUT pixelMain(float4 color : COLOR0, 
    in float3 pNormal : NORMAL0)
{
    PS_OUTPUT output;
    float3 light = normalize(diffuseDir);
    float3 norm = normalize(pNormal);
    float3 r = normalize(2 * dot(light, norm) * norm - light); //where the hell is the reflection vector?
    float3 v = normalize(mul(normalize(viewVec), world)); //where the hell is the view vector?
    float dot = dot(r, v); //checks what the angle between the camera and the reflected light is
    float4 specular = specIntensity * specColor * max(pow(dot, shine), 0) * length(color);
    output.Color = saturate(color + lightColor * lightIntensity + specular);
    return output; 
}

