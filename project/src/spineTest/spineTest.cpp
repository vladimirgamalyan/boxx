#include "SpineTest.h"
#include <spine/spine.h>
#include <vector>
#include "../spineIntegration.h"

void SpineTest::init(Graphics* graphics)
{
	_graphics = graphics;


	// Load the atlas from a file. The last argument is engine specific and will
	// load an atlas page texture and store it in the Atlas. The texture can
	// later be retrieved via Atlas->getRendererObject() by the rendering code.
	MyTextureLoader* textureLoader = new MyTextureLoader();
	textureLoader->assignGraphics(graphics);
	spine::Atlas* atlas = new spine::Atlas("assets/alien-pro/alien-pro.atlas", textureLoader);

	// Load the atlas from memory, giving the memory location, the number
	// of bytes and the directory relative to which atlas page textures
	// should be loaded. The last argument is engine specific and will
	// load an atlas page texture and store it in the Atlas. The texture can
	// later be retrieved via Atlas->getRendererObject() by the rendering code.
	//spine::Atlas* atlas = new spine::Atlas(atlasInMemory, atlasDataLengthInBytes, dir, textureLoader);

	// Create a SkeletonJson used for loading and set the scale
	// to make the loaded data two times as big as the original data
	spine::SkeletonJson json(atlas);
	json.setScale(1);

	// Load the skeleton .json file into a SkeletonData
	spine::SkeletonData* skeletonData = json.readSkeletonDataFile("assets/alien-pro/alien-pro.json");

	// If loading failed, print the error and exit the app
	if (!skeletonData) {
		printf("%s\n", json.getError().buffer());
		exit(0);
	}


    _skeleton = new spine::Skeleton(skeletonData);
    _skeleton->setScaleX(1.f);
    _skeleton->setScaleY(1.f);

	animationStateData = new spine::AnimationStateData(skeletonData);
	animationState = new spine::AnimationState(animationStateData);

	// Add the animation "walk" to track 0, without delay, and let it loop indefinitely
	animationState->addAnimation(0, "run", true, 0);
	animationState->update(0);
	animationState->apply(*_skeleton);

    _skeleton->updateWorldTransform();
}

void SpineTest::update()
{
	//animationState->update(1.0f / 60.0f);
	animationState->update(1.0f / 1000.0f);
	animationState->apply(*_skeleton);
    _skeleton->updateWorldTransform();
}

void SpineTest::draw() const
{
    if (firstLoop)
    {
        //firstLoop = false;
        drawSkeleton(_skeleton, 320, 440);
    }
}

void SpineTest::drawSkeleton(spine::Skeleton* skeleton, float x, float y) const
{
    //spine::Vector<Vertex> vertices;

    spine::Vector<float> worldVertices;

    // For each slot in the draw order array of the skeleton
    for (size_t i = 0, n = skeleton->getSlots().size(); i < n; ++i) 
    {
        spine::Slot* slot = skeleton->getDrawOrder()[i];

        // Fetch the currently active attachment, continue
        // with the next slot in the draw order if no
        // attachment is active on the slot
        spine::Attachment* attachment = slot->getAttachment();
        if (!attachment) 
            continue;

        // Fetch the blend mode from the slot and
        // translate it to the engine blend mode
        Graphics::BlendMode engineBlendMode;
        switch (slot->getData().getBlendMode()) {
        case spine::BlendMode_Normal:
            engineBlendMode = Graphics::BlendMode::BLEND_NORMAL;
            break;
        case spine::BlendMode_Additive:
            engineBlendMode = Graphics::BlendMode::BLEND_ADDITIVE;
            break;
        case spine::BlendMode_Multiply:
            engineBlendMode = Graphics::BlendMode::BLEND_MULTIPLY;
            break;
        case spine::BlendMode_Screen:
            engineBlendMode = Graphics::BlendMode::BLEND_SCREEN;
            break;
        default:
            // unknown Spine blend mode, fall back to
            // normal blend mode
            engineBlendMode = Graphics::BlendMode::BLEND_NORMAL;
        }

        // Calculate the tinting color based on the skeleton's color
        // and the slot's color. Each color channel is given in the
        // range [0-1], you may have to multiply by 255 and cast to
        // and int if your engine uses integer ranges for color channels.
        spine::Color skeletonColor = skeleton->getColor();
        spine::Color slotColor = slot->getColor();
        spine::Color tintLight(skeletonColor.r * slotColor.r, skeletonColor.g * slotColor.g, skeletonColor.b * slotColor.b, skeletonColor.a * slotColor.a);

        spine::Color tintDark;
        if (slot->hasDarkColor())
            tintDark = slot->getDarkColor();
        tintDark.a = 1.f;

        Graphics::Color4f colorLight(tintLight.r, tintLight.g, tintLight.b, tintLight.a);
        Graphics::Color4f colorDark(tintDark.r, tintDark.g, tintDark.b, tintDark.a);

        // Fill the vertices array, indices, and texture depending on the type of attachment
        void* texture = NULL;
        //unsigned short* indices = NULL;
        if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti)) {
            // Cast to an spRegionAttachment so we can get the rendererObject
            // and compute the world vertices
            spine::RegionAttachment* regionAttachment = (spine::RegionAttachment*)attachment;

            // Our engine specific Texture is stored in the AtlasRegion which was
            // assigned to the attachment on load. It represents the texture atlas
            // page that contains the image the region attachment is mapped to.
            texture = (void*)((spine::AtlasRegion*)regionAttachment->getRendererObject())->page->getRendererObject();

            // Ensure there is enough room for vertices
            worldVertices.setSize(16, 0);

            // Computed the world vertices positions for the 4 vertices that make up
            // the rectangular region attachment. This assumes the world transform of the
            // bone to which the slot (and hence attachment) is attached has been calculated
            // before rendering via Skeleton::updateWorldTransform(). The vertex positions
            // will be written directly into the vertices array, with a stride of sizeof(Vertex)
            regionAttachment->computeWorldVertices(slot->getBone(), worldVertices, 0, 2);

            /*
	            float t[2 * 3 * 4] = {
		            dstRect.w, 0,				srcRight, srcBottom,
		            dstRect.w, dstRect.h,		srcRight, srcTop,
		            0, 0,						srcLeft, srcBottom,

		            0, dstRect.h,				srcLeft, srcTop,
		            0, 0,						srcLeft, srcBottom,
		            dstRect.w, dstRect.h,		srcRight, srcTop
	            };
            */

            const float* uvs = regionAttachment->getUVs().buffer();

            float v[2 * 3 * 4] = {
                // first triangle
                worldVertices[0], worldVertices[1], uvs[0], uvs[1],
                worldVertices[2], worldVertices[3], uvs[2], uvs[3],
                worldVertices[4], worldVertices[5], uvs[4], uvs[5],

                // second triangle
                worldVertices[4], worldVertices[5], uvs[4], uvs[5],
                worldVertices[6], worldVertices[7], uvs[6], uvs[7],
                worldVertices[0], worldVertices[1], uvs[0], uvs[1]
            };

            // copy color and UVs to the vertices
            //for (size_t j = 0, l = 0; j < 4; j++, l += 2) {
            //    //Vertex& vertex = worldVertices[j];
            //    //vertex.color.set(tint);
            //    //vertex.u = regionAttachment->getUVs()[l];
            //    //vertex.v = regionAttachment->getUVs()[l + 1];

            //    worldVertices[l + 2] = regionAttachment->getUVs()[l];
            //    worldVertices[l + 3] = regionAttachment->getUVs()[l + 1];
            //}

            // set the indices, 2 triangles forming a quad
            //indices = quadIndices;

            _graphics->drawTriangles(v, 2, texture, engineBlendMode,
                x, y, 0.f, 1.f, -1.f, 0.f, 0.f, colorLight, colorDark);
        }
        else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti)) {
            // Cast to an MeshAttachment so we can get the rendererObject
            // and compute the world vertices
            spine::MeshAttachment* mesh = (spine::MeshAttachment*)attachment;



            // Our engine specific Texture is stored in the AtlasRegion which was
            // assigned to the attachment on load. It represents the texture atlas
            // page that contains the image the region attachment is mapped to.
            texture = (void*)((spine::AtlasRegion*)mesh->getRendererObject())->page->getRendererObject();

            // Computed the world vertices positions for the vertices that make up
            // the mesh attachment. This assumes the world transform of the
            // bone to which the slot (and hence attachment) is attached has been calculated
            // before rendering via Skeleton::updateWorldTransform(). The vertex positions will
            // be written directly into the vertices array, with a stride of sizeof(Vertex)
            // in fact, this is number of float, number of vertices lower as twice
            size_t numVertices = mesh->getWorldVerticesLength();

            /*
                indices = &mesh->getTriangles();
			    indicesCount = mesh->getTriangles().size();       

                for (int i = 0; i < mesh->trianglesCount; ++i) {
                    int index = mesh->triangles[i] << 1;
                        addVertex(worldVerticesPositions[index], worldVerticesPositions[index + 1],
                          mesh->uvs[index], mesh->uvs[index + 1],
                          tintR, tintG, tintB, tintA, &vertexIndex);
            }
            */

            assert(numVertices % 2 == 0);
            //size_t realNumVertices = numVertices / 2;
            //assert(realNumVertices % 3 == 0);
            //size_t triangleCount = realNumVertices / 3;

            // Ensure there is enough room for vertices
            //worldVertices.setSize(numVertices, 0);

            // numVertices = real number verticles * 2

            std::vector<float> readyVertices(mesh->getTriangles().size() * 4);

            std::vector<float> v(numVertices);
            mesh->computeWorldVertices(*slot, 0, numVertices, v.data(), 0, 2);

            int t = 0;
            for (size_t k = 0; k < mesh->getTriangles().size(); ++k)
            {
                int index = mesh->getTriangles()[k] * 2;
                readyVertices[t * 4 + 0] = v[index];
                readyVertices[t * 4 + 1] = v[index + 1];
                readyVertices[t * 4 + 2] = mesh->getUVs()[index];
                readyVertices[t * 4 + 3] = mesh->getUVs()[index + 1];

                ++t;
                //if (t == 3)
                //{
                //    t = 0;
                //    _graphics->drawTriangles(readyVertices.data(), 1, texture, engineBlendMode,
                //        x, y, 0.f, 1.f, -1.f, 0.f, 0.f);
                //}
            }



            assert(mesh->getTriangles().size() % 3 == 0);
            size_t triangleCount = mesh->getTriangles().size() / 3;


            _graphics->drawTriangles(readyVertices.data(), triangleCount, texture, engineBlendMode,
                x, y, 0.f, 1.f, -1.f, 0.f, 0.f, colorLight, colorDark);
        }

        // Draw the mesh we created for the attachment
        //engine_drawMesh(vertices, 0, vertexIndex, texture, engineBlendMode);
    }
}
