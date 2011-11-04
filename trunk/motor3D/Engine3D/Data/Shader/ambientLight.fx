// Matriz World View Projection
float4x4 wvp : WorldViewProjection;
// Intensidad de la luz
float AmbientIntensity = 1;
// Color de la luz ambiente
float4 AmbientColor : AMBIENT = float4(.5,.5,.5,1);

// Datos del vertice (entrada del vertex shader)
struct VS_INPUT
{
	float4 Position : POSITION;
};

// Salida del vertex shader
struct VS_OUTPUT
{
	float4 Position : POSITION;
};

// Salida del pixel shader
struct PS_OUTPUT
{
	float4 Color : COLOR;
};

//--------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------
VS_OUTPUT VS_Ambient(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.Position = mul(input.Position,wvp);
	
	return output;
}

//--------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------
PS_OUTPUT PS_Ambient(VS_OUTPUT input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	
	output.Color = AmbientIntensity * AmbientColor;
	
	return output;
}

//--------------------------------------------------------------------
// Simple Effect (1 technique with 1 pass)
//--------------------------------------------------------------------
technique AmbientLight
{
	pass Pass0
	{
		VertexShader = compile vp30 VS_Ambient();
		PixelShader = compile fp30 PS_Ambient();
	}
}