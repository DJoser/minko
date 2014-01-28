/*
Copyright (c) 2013 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "minko/Minko.hpp"
#include "minko/MinkoPNG.hpp"
#include "minko/MinkoJPEG.hpp"
#include "minko/MinkoSDL.hpp"

using namespace minko;
using namespace minko::component;
using namespace minko::math;

const std::string TEXTURE_2D	= "texture/box.png";
const std::string CUBE_TEXTURE	= "texture/cubemap_sea.jpeg";

int main(int argc, char** argv)
{
	auto canvas = Canvas::create("Minko Example - Sky Box", 800, 600);

	auto sceneManager = SceneManager::create(canvas->context());
	
	// setup assets
	sceneManager->assets()->defaultOptions()->resizeSmoothly(true);
	sceneManager->assets()->defaultOptions()->generateMipmaps(true);
	sceneManager->assets()
		->registerParser<file::PNGParser>("png")
		->registerParser<file::JPEGParser>("jpg")
		->registerParser<file::JPEGParser>("jpeg")
		->queue(TEXTURE_2D)
		->queue(CUBE_TEXTURE, file::Options::create(canvas->context())->isCubeTexture(true))
		->queue("effect/Basic.effect");

	sceneManager->assets()->geometry("cube", geometry::CubeGeometry::create(sceneManager->assets()->context()));
	

	auto _ = sceneManager->assets()->complete()->connect([=](file::AssetLibrary::Ptr assets)
	{
		auto cubeGeometry = geometry::CubeGeometry::create(sceneManager->assets()->context());

		auto root = scene::Node::create("root")
			->addComponent(sceneManager);

		assets->geometry("cubeGeometry", cubeGeometry);

		auto camera = scene::Node::create("camera")
			->addComponent(Renderer::create(0x7f7f7fff))
			->addComponent(Transform::create(
				Matrix4x4::create()->lookAt(Vector3::zero(), Vector3::create(0.f, 0.f, 3.f))
			))
			->addComponent(PerspectiveCamera::create(800.f / 600.f, (float)PI * 0.25f, .1f, 1000.f));
		root->addChild(camera);
		
		auto sky = scene::Node::create("sky")
			->addComponent(Transform::create(
				Matrix4x4::create()->appendScale(100.0f, 100.0f, 100.0f)
			))
			->addComponent(Surface::create(
				assets->geometry("cubeGeometry"),
				material::BasicMaterial::create()
					->diffuseCubeMap(assets->texture(CUBE_TEXTURE))
					->triangleCulling(render::TriangleCulling::NONE),
				assets->effect("effect/Basic.effect")
			));
		root->addChild(sky);

		auto box = scene::Node::create("box")
			->addComponent(Transform::create())
			->addComponent(Surface::create(
				assets->geometry("cubeGeometry"),
				material::BasicMaterial::create()
					->diffuseMap(assets->texture(TEXTURE_2D)),
				assets->effect("effect/Basic.effect")
			));
		root->addChild(box);
		
		auto resized = canvas->resized()->connect([&](AbstractCanvas::Ptr canvas, uint w, uint h)
		{
			camera->component<PerspectiveCamera>()->aspectRatio((float)w / (float)h);
		});

		auto enterFrame = canvas->enterFrame()->connect([&](Canvas::Ptr canvas, uint time, uint deltaTime)
		{
			sky->component<Transform>()->matrix()->appendRotationY(.001f);
			box->component<Transform>()->matrix()->appendRotationY(-.01f);

			sceneManager->nextFrame();
		});

		canvas->run();
	});

	sceneManager->assets()->load();

	return 0;
}


