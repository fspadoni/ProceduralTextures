// https://www.shadertoy.com/view/4sBSWW

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

// ---- 8< -------- 8< -------- 8< -------- 8< ----


float GetCurve(float x)
{
	return sin( x * 3.14159 * 4.0 );
}

float GetCurveDeriv(float x)
{
	return 3.14159 * 4.0 * cos( x * 3.14159 * 4.0 );
}

void main()
{
    
    vec2 uv = gl_FragCoord.xy;
    
	vec3 vColour = vec3(0.0);
    
	// Multiples of 4x5 work best
	vec2 vFontSize = vec2(8.0, 15.0);
    
	// Draw Horizontal Line
	if(abs(gl_FragCoord.y - iResolution.y * 0.5) < 1.0)
	{
		vColour = vec3(1.0);
	}
	
	// Draw Sin Wave
	// See the comment from iq or this page
	// http://www.iquilezles.org/www/articles/distance/distance.htm
	float fCurveX = gl_FragCoord.x / iResolution.x;
	float fSinY = (GetCurve(fCurveX) * 0.25 + 0.5) * iResolution.y;
	float fSinYdX = (GetCurveDeriv(fCurveX) * 0.25) * iResolution.y / iResolution.x;
	float fDistanceToCurve = abs(fSinY - gl_FragCoord.y) / sqrt(1.0+fSinYdX*fSinYdX);
	float fSetPixel = fDistanceToCurve - 1.0; // Add more thickness
	//vColour = mix(vec3(1.0, 0.0, 0.0), vColour, clamp(fSetPixel, 0.0, 1.0));
    

	
	
	// Print Shader Time
	vec2 vPixelCoord1 = vec2(96.0, 95.0);
	float fValue1 = iGlobalTime;
	float fDigits = 6.0;
    float fDecimalPlaces = 3.0;
	float fIsDigit1 = PrintValue( uv, vPixelCoord1, vFontSize, fValue1, fDigits, fDecimalPlaces);
	vColour = mix( vColour, vec3(0.0, 1.0, 1.0), fIsDigit1);
    
    
	if(iMouse.x > 0.0)
	{
		// Print Mouse X
		vec2 vPixelCoord2 = iMouse.xy + vec2(-52.0, 6.0);
		float fValue2 = iMouse.x / iResolution.x;
		fDigits = 1.0;
		fDecimalPlaces = 3.0;
		float fIsDigit2 = PrintValue(uv, vPixelCoord2, vFontSize, fValue2, fDigits, fDecimalPlaces);
		vColour = mix( vColour, vec3(0.0, 1.0, 0.0), fIsDigit2);
		
		// Print Mouse Y
		vec2 vPixelCoord3 = iMouse.xy + vec2(0.0, 6.0);
		float fValue3 = iMouse.y / iResolution.y;
		fDigits = 1.0;
		float fIsDigit3 = PrintValue(uv, vPixelCoord3, vFontSize, fValue3, fDigits, fDecimalPlaces);
		vColour = mix( vColour, vec3(0.0, 1.0, 0.0), fIsDigit3);
        
    }

	
    
    if(iMouse.x > 0.0)
	{
        fDigits = 1.0;
		fDecimalPlaces = 3.0;
        
		vec2 vPixelCoord = iMouse.xy + vec2(-100.0, -16.0);
        float fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, vColour.x, fDigits, fDecimalPlaces);
        vColour = mix( vColour, vec3(1.0, 0.0, 0.0), fIsDigit);
        
        vPixelCoord = iMouse.xy + vec2(-50.0, -16.0);
        fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, vColour.y, fDigits, fDecimalPlaces);
        vColour = mix( vColour, vec3(1.0, 0.0, 0.0), fIsDigit);
        
        vPixelCoord = iMouse.xy + vec2(-0.0, -16.0);
        fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, vColour.z, fDigits, fDecimalPlaces);
        vColour = mix( vColour, vec3(1.0, 0.0, 0.0), fIsDigit);
        
//        vPixelCoord = iMouse.xy + vec2( 50.0, -16.0);
//        fIsDigit = PrintValue(uv, vPixelCoord, vFontSize, gl_FragColor.w, fDigits, fDecimalPlaces);
//        vColour = mix( vColour, vec3(1.0, 0.0, 0.0), fIsDigit);
	}

    gl_FragColor = vec4(vColour,1.0);
    
}