////////////////////////////////////////////////////////////////////////////////
// Filename: font.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
#include "bufferStruct.h"

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType vsMain(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, matWorld);
    output.position = mul(output.position, matView);
    output.position = mul(output.position, matProj);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    output.color = input.color;

    return output;
}