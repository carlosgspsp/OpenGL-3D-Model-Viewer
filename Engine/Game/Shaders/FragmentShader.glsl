#version 330

//in vec4 color;
out vec4 outColor;

uniform float diffuse_constant;
uniform float specular_constant;
uniform float n;
uniform vec3 light_color;
uniform vec3 light_direction;
uniform vec3 ambient_color;
uniform vec3 camera_position;

uniform sampler2D diffuse_texture;

in vec3 surface_normal;
in vec3 surface_position;
in vec2 uv0;

void main() {
	vec3 nnormal =  normalize(surface_normal);
	vec3 nlight_direction = normalize(light_direction);
	//vec3 diffuse_color = vec3(0.3,0.3,1.0);
	vec3 diffuse_color = texture(diffuse_texture, uv0).xyz;
   //outColor = vec4(color);
   //outColor = vec4(0.3,0.3,1.0, 1.0);
   float NdotL = max(dot(nnormal, nlight_direction),0.0);
  if (NdotL > 0.0)
  {
   
   vec3 diffuse = diffuse_constant * diffuse_color * light_color * NdotL;
   //vec3 V = normalize(camera_position-surface_position);
   vec3 V = normalize(surface_position-camera_position);
   vec3 R = reflect(nlight_direction,nnormal);
   //vec3 R = reflect(nnormal , nlight_direction);
   float RdotV = max(dot(R,V),0.0);
   vec3 specular = specular_constant * light_color * pow(RdotV,n);
   
   vec3 color = ambient_color*diffuse_color + diffuse + specular; 
   outColor = vec4(color, 1.0);
   //outColor = vec4(ambient_color*diffuse_color + diffuse_constant*diffuse_color*light_color*max(dot(nnormal, nlight_direction),0.0) + specular_constant * light_color * pow(max(dot(V,R),0.0),n),1.0);
   
  }else{
  
  outColor =  vec4(diffuse_color*ambient_color,1.0);
  }

   
}