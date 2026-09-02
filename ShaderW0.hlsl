
Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

// ShaderW0.hlsl
cbuffer constants : register(b0)
{
	float3 Offset;
	float Scale;
    float4 Color;
}

struct VS_INPUT
{
	float3 position : POSITION; // 정점 버퍼로부터의 위치 입력
	float4 color : COLOR; // 정점 버퍼로 부터의 색상 입력
    float2 textureCoordinate : TEXCOORD;
};

struct PS_INPUT
{
	float4 position : SV_POSITION; // 픽셀 셰이더로 패스하기 위한 변환된 위치
	float4 color : COLOR; // 픽셀 셰이더로 패스하기 위한 색상
    float2 textureCoordinate : TEXCOORD;
};

PS_INPUT mainVS(VS_INPUT input)
{
	PS_INPUT output;

	// 픽셀셰이더로 직접 위치를 패스 (변환 없이)
	// output.position = input.position;

	float4 scaledPosition = float4(input.position.xyz * Scale, 1.0f);
	output.position = float4(Offset, 0.0f) + scaledPosition;

	// 픽셀셰이더로 색상 패스
    output.color = Color;
    output.textureCoordinate = input.textureCoordinate;
	
	return output;
}

float4 mainPS(PS_INPUT input) : SV_TARGET
{
	/*
	//return input.color;
    float4 textureColor = gTexture.Sample(gSampler, input.textureCoordinate);
	
	// Todo: Fix hack
	// 감마 중복 적용을 상쇄하기 위해 2.2 제곱 적용
    float3 correctedColor = pow(textureColor.rgb, 2.2f);
    return float4(correctedColor, textureColor.a);
	*/
	
    float4 textureColor = gTexture.Sample(gSampler, input.textureCoordinate);

    // 알파 값이 0.1 미만이면 해당 픽셀 렌더링을 즉시 중단(버림)
    clip(textureColor.a - 0.1f);

    float3 correctedColor = pow(textureColor.rgb, 2.2f);
    return float4(correctedColor, textureColor.a);
}