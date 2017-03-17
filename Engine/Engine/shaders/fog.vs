//Filename: fog.vs

cbuffer PerFrameBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer FogBuffer
{
    float fogStart;
    float fogEnd;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float fogFactor : FOG;
};

PixelInputType FogVertexShader(VertexInputType input)
{
    PixelInputType output;
    float4 cameraPosition;

    //change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    //calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    //calculate the camera position.
    cameraPosition = mul(input.position, worldMatrix);
    cameraPosition = mul(cameraPosition, viewMatrix);

    //calculate linear fog.
    output.fogFactor = saturate((fogEnd - cameraPosition.z) / (fogEnd - fogStart));

    return output;
}