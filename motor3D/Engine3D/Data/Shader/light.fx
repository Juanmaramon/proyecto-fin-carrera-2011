
//-----------------------------------------------------------------------------
// Variables del efecto
//-----------------------------------------------------------------------------

// Matriz WVP
float4x4 worldViewProj;
float4x4 world;
float4x4 worldInverseTranspose;

// Direccion de la luz
//float3 LightDirection : Direction = float3(0,50,10);
float3 LightDirection : Direction = float3(0,1,1);
//float3 LightDirection : Direction = float3(-1,5,0);


// Color de la luz de ambiente
float4 AmbientColor : Ambient = float4(.0,.0,.0,1);

// Posicion camara
float3 cameraPos;

// Intensidad de la luz
float AmbientIntensity = 1;

// Textura difuso
sampler2D Diffuse_0 = sampler_state {
	minFilter = LinearMipMapLinear;
	magFilter = Linear;
	AddressU = Repeat;
	AddressV = Repeat;
};

// Textura especular
sampler Specular_0 = sampler_state
{
	minFilter = LinearMipMapLinear;
	magFilter = Linear;
	AddressU = Repeat;
	AddressV = Repeat;   
};

// Textura especular
sampler Normals_0 = sampler_state
{
	minFilter = LinearMipMapLinear;
	magFilter = Linear;
	AddressU = Repeat;
	AddressV = Repeat;   
};

// Datos del vertice
struct VS_INPUT
{
	float3 position : POSITION;
	float2 tex0 	: TEXCOORD0;
	float3 Normal : NORMAL;
	float3 Tangent : TEXCOORD1;
};

// Datos de salida del vertex shader
struct VS_OUTPUT
{
    	float4 position : POSITION;
	float2 tex0 : TEXCOORD0;
	float3 Light : TEXCOORD1;
	float3 CamView : TEXCOORD2;
    	float4 posS : TEXCOORD3;
	float3 Normal : TEXCOORD4;
};

// Salida del pixel shader
struct PS_OUTPUT
{
	float4 Color : COLOR;
};

//-----------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------
VS_OUTPUT myvs( const VS_INPUT IN )
{
	VS_OUTPUT OUT;

	float4 position = float4(IN.position, 1.0);
	OUT.position = mul( worldViewProj, position );

	float3x3 worldToTangentSpace;
  	worldToTangentSpace[0] = mul((float3x3)world, IN.Tangent);
    	worldToTangentSpace[1] = mul((float3x3)world, cross(IN.Tangent, IN.Normal));
    	worldToTangentSpace[2] = mul((float3x3)world, IN.Normal);

	// Diffuse lightning and specular
	OUT.Light = normalize(mul(worldToTangentSpace, LightDirection));

	OUT.tex0 = IN.tex0;

	OUT.CamView = normalize(cameraPos - mul( world, position ));
	OUT.posS = position;

	// Bump
	OUT.Normal = normalize(mul((float3x3)world, IN.Normal));

	return OUT;
}


//-----------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------
PS_OUTPUT myps( VS_OUTPUT IN )
{
	PS_OUTPUT output;
	
	// Bump
	float3 Normal = (2 * (tex2D(Normals_0, IN.tex0))) - 1.0;
	
	float Diffuse = saturate(dot(IN.Light, Normal));
	float4 Ambient = AmbientIntensity * AmbientColor;
	float4 texCol = tex2D(Diffuse_0, IN.tex0);

	texCol *= Diffuse;
	
	float3 Half = normalize(normalize(LightDirection) + normalize(IN.CamView));
	float specular = pow(saturate(dot(IN.Normal, Half)), 8);
	float4 specCol = 2 * tex2D(Specular_0, IN.tex0) * (specular * Diffuse);

	output.Color = Ambient + texCol + specCol;

	return output;
}

//-----------------------------------------------------------------------------
// Simple Effect (1 technique with 1 pass)
//-----------------------------------------------------------------------------
technique Technique0
{
    pass Pass0
    {
        Zenable  = true;
        CullFaceEnable = true;

        VertexShader = compile arbvp1 myvs();
        PixelShader  = compile arbfp1 myps();
    }
}
