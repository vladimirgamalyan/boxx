uniform vec4 light = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 dark = vec4(0.0, 0.0, 0.0, 0.0);
uniform sampler2D textureData;

in vec2 interpolatedTextureCoordinates;

out vec4 fragmentColor;

void main() {
	vec4 texColor = texture2D(textureData, interpolatedTextureCoordinates);
	float alpha = texColor.a * light.a;
	fragmentColor.a = alpha;	
	fragmentColor.rgb = ((texColor.a - 1.0) * dark.a + 1.0 - texColor.rgb) * dark.rgb + texColor.rgb * light.rgb;
}
