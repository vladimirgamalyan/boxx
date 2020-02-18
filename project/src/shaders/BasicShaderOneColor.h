#pragma once
#include <Magnum/GL/AbstractShaderProgram.h>
#include <Magnum/GL/Texture.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Math/Matrix3.h>

class BasicShaderOneColor: public Magnum::GL::AbstractShaderProgram
{
public:
    typedef Magnum::GL::Attribute<0, Magnum::Vector2> Position;
    typedef Magnum::GL::Attribute<1, Magnum::Vector2> TextureCoordinates;

    explicit BasicShaderOneColor();

	BasicShaderOneColor& bindTexture(Magnum::GL::Texture2D& texture) {
		texture.bind(TextureLayer);
		return *this;
	}

	BasicShaderOneColor& setColor(const Magnum::Color4& color) {
		setUniform(_colorUniform, color);
		return *this;
	}

	BasicShaderOneColor& setUvShift(const Magnum::Vector2& shift) {
		setUniform(_uvShift, shift);
		return *this;
	}

	BasicShaderOneColor& setUvMult(const Magnum::Vector2& mult) {
		setUniform(_uvMult, mult);
		return *this;
	}

	BasicShaderOneColor& setTransformationProjectionMatrix(const Magnum::Matrix3& matrix) {
		setUniform(_transformationProjectionMatrixUniform, matrix);
		return *this;
	}

private:
    enum: Magnum::Int { TextureLayer = 0 };

	Magnum::Int _colorUniform;
	Magnum::Int _transformationProjectionMatrixUniform;
	Magnum::Int _uvShift;
	Magnum::Int _uvMult;
};
