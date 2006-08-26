uniform sampler2DShadow shadowMap;

void main()
{

 float s0 ;
 const float sc = 1.0/512.0;
 
 s0 = shadow2DProj(shadowMap, gl_TexCoord[1]).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(-sc, -sc, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(-sc, 0, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(-sc, sc, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(sc, -sc, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(sc, 0, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(sc, sc, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(0, -sc, 0, 0)*gl_TexCoord[1].q).r;
 s0 += shadow2DProj(shadowMap, gl_TexCoord[1]+vec4(0, sc, 0, 0)*gl_TexCoord[1].q).r;
 
 s0  = s0 / 9.0;
 s0 = (1.0 - s0)*0.64;
 
 gl_FragColor = vec4(s0, s0, s0, s0);          
}