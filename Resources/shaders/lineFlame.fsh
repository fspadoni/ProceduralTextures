

uniform float iGlobalTime;
uniform vec2 iResolution;
uniform float iFreq;
uniform float iAmp;
uniform int iNumberOctave;
uniform vec2 iMouse;

//uniform float time;
//uniform vec2 screenSize;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying lowp vec2 v_texCoord;
#else
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
#endif



float DigitBin(const in int x)
{
    return x==0?480599.0:x==1?139810.0:x==2?476951.0:x==3?476999.0:x==4?350020.0:x==5?464711.0:x==6?464727.0:x==7?476228.0:x==8?481111.0:x==9?481095.0:0.0;
}

float PrintValue(const in vec2 fragCoord, const in vec2 vPixelCoords, const in vec2 vFontSize, const in float fValue, const in float fMaxDigits, const in float fDecimalPlaces)
{
    vec2 vStringCharCoords = (fragCoord.xy - vPixelCoords) / vFontSize;
    if ((vStringCharCoords.y < 0.0) || (vStringCharCoords.y >= 1.0)) return 0.0;
	float fLog10Value = log2(abs(fValue)) / log2(10.0);
	float fBiggestIndex = max(floor(fLog10Value), 0.0);
	float fDigitIndex = fMaxDigits - floor(vStringCharCoords.x);
	float fCharBin = 0.0;
	if(fDigitIndex > (-fDecimalPlaces - 1.01)) {
		if(fDigitIndex > fBiggestIndex) {
			if((fValue < 0.0) && (fDigitIndex < (fBiggestIndex+1.5))) fCharBin = 1792.0;
		} else {
			if(fDigitIndex == -1.0) {
				if(fDecimalPlaces > 0.0) fCharBin = 2.0;
			} else {
				if(fDigitIndex < 0.0) fDigitIndex += 1.0;
				float fDigitValue = (abs(fValue / (pow(10.0, fDigitIndex))));
                float kFix = 0.0001;
                fCharBin = DigitBin(int(floor(mod(kFix+fDigitValue, 10.0))));
			}
		}
	}
    return floor(mod((fCharBin / pow(2.0, floor(fract(vStringCharCoords.x) * 4.0) + (floor(vStringCharCoords.y * 5.0) * 4.0))), 2.0));
}


vec2 rand(vec2 co)
{
    return texture2D(CC_Texture0, co).rg;
    float x=fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
    float y=fract(sin(dot(co.xy+vec2(3223.21323,-3323.312323) ,vec2(12.9898,78.233))) * 43758.5453);
    return vec2(x,y);
}

vec3 hash( vec3 p )
{
	//p = vec3( dot(p,vec3(127.1,311.7, 234.6)), dot(p,vec3(269.5,183.3,198.3)),  dot(p,vec3(169.5,283.3,156.9)));
    
	return -1.0 + 2.0*fract( sin(p)*43758.5453123 );
}

float blend(float x){
	float x2=x*x;
	float x3=x*x2;
	return 6.*x3*x2-15.*x2*x2+10.*x3;
}

float fnoisePerlin(float amp, float freq, float x, float y, float z)
{
	x=x*freq;
	y=y*freq;
	float fx=floor(x);
	float fy=floor(y);
	float fz=floor(z);
	float cx=ceil(x);
	float cy=ceil(y);
	float cz=ceil(z);
    
	vec3 v000=hash(vec3(fx, fy, fz));
	vec3 v100=hash(vec3(cx, fy, fz));
	vec3 v010=hash(vec3(fx, cy, fz));
	vec3 v110=hash(vec3(cx, cy, fz));
	vec3 v001=hash(vec3(fx, fy, cz));
	vec3 v101=hash(vec3(cx, fy, cz));
	vec3 v011=hash(vec3(fx, cy, cz));
	vec3 v111=hash(vec3(cx, cy, cz));
    
    
	float a000=dot(v000, vec3(x-fx,y-fy, z-fz));
	float a100=dot(v100, vec3(x-cx,y-fy, z-fz));
	float a010=dot(v010, vec3(x-fx,y-cy, z-fz));
	float a110=dot(v110, vec3(x-cx,y-cy, z-fz));
	
    float a001=dot(v001, vec3(x-fx,y-fy, z-cz));
	float a101=dot(v101, vec3(x-cx,y-fy, z-cz));
	float a011=dot(v011, vec3(x-fx,y-cy, z-cz));
	float a111=dot(v111, vec3(x-cx,y-cy, z-cz));
    
    
    float mx=blend(x-fx);
    float my=blend(y-fy);
    float mz=blend(z-fz);
    
    
    float ix00=mix(a000, a100, mx);
    float ix10=mix(a010, a110, mx);
    float ix01=mix(a001, a101, mx);
    float ix11=mix(a011, a111, mx);
    
    float iy0=mix(ix00,ix10, my);
    float iy1=mix(ix01,ix11, my);
    
    float iz=mix(iy0, iy1, mz);
    
	return amp*iz;// sin(amp*iz);
}


float pt(float t)
{
    vec2 uv = gl_FragCoord.xy;
    
    float f=0.;
    float freq = iAmp; //0.0065;
    float amp = iFreq; //10.5;
    
    for (int i=0; i<iNumberOctave; i++)
    {
        f = fnoisePerlin(iAmp, iFreq, uv.x,uv.y, 2.0*3.1415*t);
        amp *= 0.5; // 0.15;
        freq *= 2.0;
    }
    
    return 1.0 -abs(f*3.0 + uv.y/iResolution.y *100.0 - 50.0);
}

void main()
{
	
    //f=pow(f*0.045, 1.)*1.;
//    vec2 uv = gl_FragCoord.xy/iResolution.xy;
//    vec3 a=vec3(1.,1.,1.);
//    vec3 b=vec3(0.,0.,0.);
//    vec3 v3=mix(a,b, uv.y+pt(1.0*iGlobalTime));
//	gl_FragColor=vec4(v3, 1.);
    float t= pt(iGlobalTime);
    vec3 color = vec3(t, t, t);
    
    if(iMouse.x > 0.0)
	{
        vec2 uv = gl_FragCoord.xy;
        // Multiples of 4x5 work best
        vec2 vFontSize = vec2(8.0, 15.0);

        float fDigits = 2.0;
		float fDecimalPlaces = 4.0;
        
		vec2 vPixelCoord = iMouse.xy + vec2(-100.0, -16.0);
        float fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, color.x, fDigits, fDecimalPlaces);
        color = mix( color, vec3(1.0, 0.0, 0.0), fIsDigit);
        
//        vPixelCoord = iMouse.xy + vec2(-50.0, -16.0);
//        fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, color.y, fDigits, fDecimalPlaces);
//        color = mix( color, vec4(1.0, 0.0, 0.0, 1.0), fIsDigit);
//        
//        vPixelCoord = iMouse.xy + vec2(-0.0, -16.0);
//        fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, color.z, fDigits, fDecimalPlaces);
//        color = mix( color, vec4(1.0, 0.0, 0.0, 1.0), fIsDigit);
//        
//        vPixelCoord = iMouse.xy + vec2( 50.0, -16.0);
//        fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, color.w, fDigits, fDecimalPlaces);
//        color = mix( color, vec4(1.0, 0.0, 0.0, 1.0), fIsDigit);
        
	}

    gl_FragColor = vec4(color,1.0);
    
}