uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
uniform sampler2D textureData;

in vec2 interpolatedTextureCoordinates;

out vec4 fragmentColor;

void main() {
	fragmentColor.rgba = color * texture(textureData, interpolatedTextureCoordinates).rgba;
}
