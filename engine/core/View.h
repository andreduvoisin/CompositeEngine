/*!
  @file       View.h
  @brief      View class definition

  @author     rmcinnis@solidice.com
  @date       june 01, 2019
  @par        copyright (c) 2019 ARK engineering
*/
#pragma once

#include <SDL.h>
#include <SDL_syswm.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "graphics/ceasset/input/AssetImporter.h"
#include <vector>
#include "graphics/animation/AnimationComponent.h"
#include "graphics/animation/AnimationManager.h"
#include "graphics/mesh/Mesh.h"
#include "graphics/mesh/Vertex.h"
#include "graphics/mesh/MeshManager.h"
#include "graphics/mesh/MeshComponent.h"
#include "graphics/skeleton/Skeleton.h"
#include "graphics/skeleton/SkeletonManager.h"
#include "graphics/texture/TextureManager.h"
#include "graphics/texture/Texture.h"
#include "core/Engine.h"
#include "core/FpsCounter.h"
#include "core/Camera.h"
#include "core/EditorCameraEventHandler.h"
#include "cef/CefMain.h"

namespace CE
{
  // high level interface to opengl based ui calls
  // will be instantiated within the core, thereby being the only instance
  //
  class View
  {
    protected:
      const int SCREEN_WIDTH = 1280;
      const int SCREEN_HEIGHT = 720;

      SDL_Window* _window = nullptr;
      SDL_GLContext _context = nullptr;

      bool _renderQuad = true;

      GLuint _skinnedMeshDiffuseTextureProgramId = 0;
      GLuint _vbo = 0;
      GLuint _ibo = 0;
      GLuint _vao = 0;
      GLuint _tbo = 0;

      GLuint _skeletonProgramId = 0;
      GLuint _uiProgramId = 0;
      GLuint _gridProgramId = 0;
      GLuint _skinnedMeshWireFrameDiffuseTextureProgramId = 0;

      GLuint _skinnedMeshDiffuseTextureProjectionViewModelMatrixId = -1;
      GLuint _skinnedMeshDiffuseTexturePaletteId = -1;
      GLuint _paletteTextureUnit = -1;
      GLuint _paletteGenTex = -1;
      GLuint _skinnedMeshDiffuseTextureDiffuseTextureId = -1;
      GLuint _diffuseTextureUnit = -1;
      GLuint _diffuseTextureID = -1;

      GLuint _skeletonProjectionViewModelMatrixId = -1;
      GLuint _skeletonPaletteId = -1;

      GLuint _skinnedMeshWireFrameDiffuseTextureProjectionViewModelMatrixId = -1;
      GLuint _skinnedMeshWireFrameDiffuseTexturePaletteId = -1;
      GLuint _skinnedMeshWireFrameDiffuseTextureDiffuseTextureId = -1;

      GLuint _gridProjectionViewModelMatrixId = -1;

      GLuint _uiTextureId = -1;
      GLuint _uiTextureUnit = -1;
      GLuint _uiTextureID = -1;

      CE::AssetImporter* _assetImporter = nullptr ;
      std::vector<CE::MeshComponent*> _meshComponents;
      std::vector<CE::AnimationComponent*> _animationComponents;
      CE::CefMain* _cefMain = nullptr;
      EventSystem* _eventSystem = nullptr;
      CE::Engine* _engine = nullptr;
      CE::FpsCounter* _fpsCounter = nullptr;
      CE::Camera* _camera = nullptr;
      CE::EditorCameraEventHandler* _editorCameraEventHandler = nullptr;

      bool           _initialized = false;
      bool           _quit = false;

      GLuint         createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName);
      GLuint         createShader(GLenum shaderType, const char* shaderFileName);
      bool           initializeOpenGL();
      void           printProgramLog(GLuint program);
      void           printShaderLog(GLuint shader);
      void           processEvent(SDL_Event& event);
      void           render();
      void           renderGrid(const glm::mat4& projectionViewModel);
      void           renderHUD();
      void           renderMesh(CE::MeshComponent& meshComponent, CE::AnimationComponent& animationComponent, const glm::mat4& projectionViewModel);
      void           renderObjects(const glm::mat4& projectionViewModel);
      void           renderSkeleton(CE::AnimationComponent& animationComponent, const glm::mat4& projectionViewModel);

    public :
                     View();
                    ~View();

      void           destroy();
      bool           initialize(int argc, char* argv[]);
      int            runLoop();
      void           stop();
      void           update(float dwDeltaSeconds, uint64_t currentTicks);

      // access methods
      bool           isInitialized() { return _initialized; }
      EventSystem   *eventSystem() { return _eventSystem; }
      SDL_Window    *window() { return _window; }
      FpsCounter    *fpsCounter() { return _fpsCounter; }
      Camera        *camera() { return _camera; }
      int            renderMode() const { return _engine->GetRenderMode();  }

  }; // class View

} // namespace: CE