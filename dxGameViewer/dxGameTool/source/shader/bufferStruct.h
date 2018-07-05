#pragma once

cbuffer CameraBuffer : register (b0)
{
	float4x4 matView;
	float4x4 matProj;
};

cbuffer TransBuffer : register (b1)
{
	float4x4 matWorld;
};

cbuffer ColorBuffer : register (b2)
{
	float4 fColor;
};

cbuffer BoneBuffer : register (b3)
{
	float4x4 matBones[128];
};



static float4x4 IdentityMatrix =
{
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, 0, 0, 1 }
};