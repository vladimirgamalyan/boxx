layout(location = 0) in highp vec2 position;
layout(location = 1) in vec2 textureCoordinates;

uniform mat3 transformationProjectionMatrix;
uniform vec2 uvShift = vec2(0.0, 0.0);
uniform vec2 uvMult = vec2(1.0, 1.0);

out vec2 interpolatedTextureCoordinates;

void main() {
    interpolatedTextureCoordinates = textureCoordinates * uvMult + uvShift;
	gl_Position.xywz = vec4(transformationProjectionMatrix * vec3(position, 1.0), 0.0);
}
