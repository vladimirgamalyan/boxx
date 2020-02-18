#pragma once
#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>

class BasicShader : public Magnum::GL::AbstractShaderProgram
{
public:
	typedef Magnum::GL::Attribute<0, Magnum::Vector2> Position;
	typedef Magnum::GL::Attribute<1, Magnum::Vector2> TextureCoordinates;
	typedef Magnum::GL::Attribute<2, Magnum::Vector4> Color;

	explicit BasicShader();

	BasicShader& bindTexture(Magnum::GL::Texture2D& texture) {
		texture.bind(TextureLayer);
		return *this;
	}

	BasicShader& setUvShift(const Magnum::Vector2& shift) {
		setUniform(_uvShift, shift);
		return *this;
	}

	BasicShader& setUvMult(const Magnum::Vector2& mult) {
		setUniform(_uvMult, mult);
		return *this;
	}

	BasicShader& setTransformationProjectionMatrix(const Magnum::Matrix3& matrix) {
		setUniform(_transformationProjectionMatrixUniform, matrix);
		return *this;
	}

private:
	enum : Magnum::Int { TextureLayer = 0 };

	Magnum::Int _transformationProjectionMatrixUniform;
	Magnum::Int _uvShift;
	Magnum::Int _uvMult;
};
