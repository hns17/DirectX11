////////////////////////////////////////////////////////////////////////////////
// Filename: basic.vs
////////////////////////////////////////////////////////////////////////////////
#include "bufferStruct.h"

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
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
		
    
	// Store the input color for the pixel shader to use.
    //output.color = float4(1.f,1.f,1.f,1.f);
    output.color = input.color;

    return output;
}