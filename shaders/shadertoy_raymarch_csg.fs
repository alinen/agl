// Copyright (c) 2020, Savvy Sine, Aline Normoyle, MIT License
#version 400

uniform vec2 iResolution;
uniform float iGlobalTime;
out vec4 FragColor;

// http://byteblacksmith.com/
//   improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/
// watch out for float precision

float noiseVec3(in vec3 p) {
  return fract(sin(dot(p,vec3(12.9898f, 78.233f, 1.87124f))) * 43758.5453f);
}

float sdSphere(in vec3 p, in float s) {
  return length(p)-s;
}

// from iq: https://www.shadertoy.com/view/Xds3zN
float sdBox(vec3 p, vec3 b) {
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) + length(max(d,0.0));
}

// from iq: https://www.shadertoy.com/view/Xds3zN
float sdRoundCone( in vec3 p, in float r1, float r2, float h ) {
  vec2 q = vec2( length(p.xz), p.y );

  float b = (r1-r2)/h;
  float a = sqrt(1.0-b*b);
  float k = dot(q,vec2(-b,a));
    
  if( k < 0.0 ) return length(q) - r1;
  if( k > a*h ) return length(q-vec2(0.0,h)) - r2;
        
  return dot(q, vec2(a,b) ) - r1;
}

float opU(float d1, float d2) {
  return min(d1, d2); 
}

float opSub(float d1, float d2) {
  return max(-d1, d2); 
}

vec3 rotate(in vec3 p, float theta, float psi)
{
  mat3 Rz = mat3(vec3(cos(theta), -sin(theta), 0),
                 vec3(sin(theta),  cos(theta), 0),
                 vec3(0,0,1));

  mat3 Ry = mat3(vec3(cos(theta), 0, sin(theta)),
                 vec3(0,1,0),
                 vec3(-sin(theta), 0, cos(theta)));
  return Ry * Rz * p; 
}

float object(in vec3 p) {
  float cs = 2.0f;
  vec3 cell = floor(abs(p+1.0f)/cs) * cs;
  float random = noiseVec3(cell);
  float size = clamp(0.5f+random, 0.1f, 1.0f);
  vec2 offset = 0.25f * vec2(random, 1.0f-random);

  p.xy = mod(p.xy+offset+1.0f, 2.0f)-1.0f;
  p.z = p.z < 0.0f? mod(p.z, 3.0f) : p.z;
  p = rotate(p, iGlobalTime*size, iGlobalTime + 0.2f * size);

  float r1 = 0.5f * size;
  float r2 = 0.25f * size;
  float s = sdSphere(p, r1);
  float h = min(s, sdSphere(p-vec3(0,r1,0), r2));
  h = min(h, sdSphere(p+vec3(0,r1,0), r2));
  h = min(h, sdSphere(p-vec3(r1,0, 0), r2));
  h = min(h, sdSphere(p+vec3(r1,0, 0), r2));
  h = min(h, sdSphere(p-vec3(0,0,r1), r2));
  h = min(h, sdSphere(p+vec3(0,0,r1), r2));
  return h;
}

// adapted from iq: https://www.shadertoy.com/view/Xds3zN
float raycast( in vec3 ro, in vec3 rd) {
  float res = -1.0;

  float tmin = 0.1;
  float tmax = 100.0;

  // raymarch primitives   
  float t = tmin;
  for( int i=0; i<70 && t<tmax; i++ ) {
    vec3 p = ro + rd * t;
    float h = object(p); 
    if( abs(h)<(0.0001*t) ) { 
      res = t; 
      break;
    }
    t += h;
  }
    
  return res;
}

vec3 normal(in vec3 p) {
  float eps = 0.001;
  vec2 h = vec2(eps, 0.0);
  float dx = object(p+h.xyy) - object(p-h.xyy);
  float dy = object(p+h.yxy) - object(p-h.yxy);
  float dz = object(p+h.yyx) - object(p-h.yyx);
  return normalize(vec3(dx, dy, dz));
}

vec3 render( in vec3 ro, in vec3 rd) {
  vec3 colors[4] = vec3[](
        vec3(0xFF, 0xc9, 0x96),
        vec3(0xFF, 0x84, 0x74),
        vec3(0x9f, 0x5f, 0x80),
        vec3(0x58, 0x3d, 0x72)
  );

  // background
  vec3 background = vec3(0.7, 0.7, 0.9) - max(rd.y,0.0)*0.3;
  vec3 col = background;

  // raycast scene
  float t = raycast(ro,rd);
  if (t > 0.0) {
    vec3 p = ro + rd * t; 
    vec3 n = normal(p);
    vec3 lpos = normalize( vec3(-0.5, 0.4, 0.6) );

    float cs = 2.0;
    vec3 cell = floor(abs(p+1.0)/cs) * cs;
    float random = abs(noiseVec3(cell));
    vec3 color = colors[int(floor(mod(4.0 * random, 4.0)))] / 255.0;

    col = max(0.0, dot(lpos, n)) * color; //vec3(1.0, 0.0, 0.5);

    col = mix(col, background, min(1.0, abs((p.z-ro.z)/30.0)));
  }

  return vec3( clamp(col,0.0,1.0) );
}

void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
  vec2 screenp = (4.0*fragCoord-2.0*iResolution.xy)/iResolution.y;
  vec3 ro = vec3(cos(0.3*iGlobalTime), sin(0.7*iGlobalTime), 4.0 - iGlobalTime);
  vec3 p = vec3(ro.xy + screenp, -iGlobalTime);

  vec3 rd = normalize(p - ro);

  vec3 col = render( ro, rd );
  col = pow( col, vec3(0.4545) );  // gamma

  fragColor = vec4( col, 1.0 );
}

void main() {
  vec4 fragColor = vec4(1,0,0,1);
  mainImage(fragColor, gl_FragCoord.xy);
  FragColor = fragColor;
}
