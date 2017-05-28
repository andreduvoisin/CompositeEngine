// A simple pixel shader.


struct PixelShaderInput
{
	float4 color : COLOR;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
	return IN.color;
}
