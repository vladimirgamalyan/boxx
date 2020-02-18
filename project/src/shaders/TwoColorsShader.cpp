#include "TwoColorsShader.h"
#include <Corrade/Containers/Reference.h>
#include <Corrade/Utility/Resource.h>
#include <Magnum/GL/Context.h>
#include <Magnum/GL/Shader.h>
#include <Magnum/GL/Version.h>

TwoColorsShader::TwoColorsShader()
{
    MAGNUM_ASSERT_GL_VERSION_SUPPORTED(Magnum::GL::Version::GL330);

    const Magnum::Utility::Resource rs{ "shader-data" };

    Magnum::GL::Shader vert{ Magnum::GL::Version::GL330, Magnum::GL::Shader::Type::Vertex };
    Magnum::GL::Shader frag{ Magnum::GL::Version::GL330, Magnum::GL::Shader::Type::Fragment };

    vert.addSource(rs.get("TwoColorsShader.vert"));
    frag.addSource(rs.get("TwoColorsShader.frag"));

    CORRADE_INTERNAL_ASSERT_OUTPUT(Magnum::GL::Shader::compile({ vert, frag }));

    attachShaders({ vert, frag });

    CORRADE_INTERNAL_ASSERT_OUTPUT(link());

    _lightUniform = uniformLocation("light");
    _darkUniform = uniformLocation("dark");
    _transformationProjectionMatrixUniform = uniformLocation("transformationProjectionMatrix");
    _uvShift = uniformLocation("uvShift");
    _uvMult = uniformLocation("uvMult");

    setUniform(uniformLocation("textureData"), TextureLayer);
}
