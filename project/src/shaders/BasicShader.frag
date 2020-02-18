uniform sampler2D textureData;

in vec2 interpolatedTextureCoordinates;
in vec4 vertexColor;

out vec4 fragmentColor;

void main() {
	fragmentColor.rgba = vertexColor * texture(textureData, interpolatedTextureCoordinates).rgba;
}
