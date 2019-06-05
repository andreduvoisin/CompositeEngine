/*!
  @file       View.cpp
  @brief      View class implementation

  @author     rmcinnis@solidice.com
  @date       june 01, 2019
  @par        copyright (c) 2019 ARK engineering
*/
#include "View.h" 

#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>

#include <glm/gtx/matrix_decompose.hpp>

#include "common/debug/AssertThread.h"
#include "core/clock/RealTimeClock.h"
#include "core/clock/GameTimeClock.h"
#include "event/ToggleBindPoseEvent.h"
#include "event/SetRenderModeEvent.h"
#include "event/SdlEvent.h"
#include "core/EditorCameraEventHandler.h"
#include "event/WindowsMessageEvent.h"

namespace CE
{
  std::vector<const char*> g_assetNames = {
    "assets/Quarterback Pass.ceasset",       // vertices: 8942, indices: 41730,  fps: 90
    "assets/Thriller Part 2.ceasset",        // vertices: 7726, indices: 39081,  fps: 82
//    "assets/Standing Walk Forward.ceasset"  // vertices: 12384, indices: 63018,  fps: 10
    "assets/Zombie Stand Up.ceasset"        // vertices: 4659, indices: 22344,  fps: 46
  };

  std::string ReadFile(const char* file)
  {

#ifdef __APPLE__
    std::string fileString(file);

    std::string directoryName;
    std::string fileName;
    std::string fileTitle;
    std::string fileExtension;

    size_t slashPos = fileString.find_last_of('/');
    if (slashPos == std::string::npos)
    {
      directoryName = std::string();
      fileName = fileString;
    }
    else
    {
      directoryName = fileString.substr(0, slashPos);
      fileName = fileString.substr(slashPos + 1);
    }
    size_t dotPos = fileName.find_last_of('.');
    fileTitle = fileName.substr(0, dotPos);
    fileExtension = fileName.substr(dotPos + 1);

    // TODO: Free? See https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFMemoryMgmt/Concepts/Ownership.html#//apple_ref/doc/uid/20001148-103029
    // Also do this where this logic is copy/pasted.
    CFStringRef fileTitleStringRef = CFStringCreateWithCStringNoCopy(NULL, fileTitle.c_str(), kCFStringEncodingASCII, kCFAllocatorNull);
    CFStringRef fileExtensionStringRef = CFStringCreateWithCStringNoCopy(NULL, fileExtension.c_str(), kCFStringEncodingASCII, kCFAllocatorNull);
    CFStringRef directoryNameStringRef = directoryName.empty() ? NULL : CFStringCreateWithCStringNoCopy(NULL, directoryName.c_str(), kCFStringEncodingASCII, kCFAllocatorNull);

    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef fileUrl = CFBundleCopyResourceURL(
      mainBundle,
      fileTitleStringRef,
      fileExtensionStringRef,
      directoryNameStringRef);

    UInt8 realFileName[1024];
    CFURLGetFileSystemRepresentation(fileUrl, true, realFileName, 1024);
#else
    const char* realFileName = file;
#endif

    std::ifstream stream((const char*)realFileName);

    if (!stream.is_open())
    {
      printf("Error Opening File: %s\n", realFileName);
      return std::string();
    }

    std::stringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
  } // :: ReadFile

  GLuint View::createProgram(const char* vertexShaderFileName, const char* fragmentShaderFileName)
  {
    GLuint programId = glCreateProgram();

    GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderFileName);
    if (vertexShader == -1)
    {
      return -1;
    }
    glAttachShader(programId, vertexShader);

    GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderFileName);
    if (fragmentShader == -1)
    {
      return -1;
    }
    glAttachShader(programId, fragmentShader);

    glLinkProgram(programId);

    printProgramLog(programId);

    GLint programSuccess = GL_TRUE;
    glGetProgramiv(programId, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE)
    {
      printf(
        "error linking program %d for shaders %s and %s\n",
        programId,
        vertexShaderFileName,
        fragmentShaderFileName);
      return -1;
    }

    return programId;
  } // View::createProgram

  GLuint View::createShader(GLenum shaderType, const char* shaderFileName)
  {
    GLuint shader = glCreateShader(shaderType);

    std::string shaderSource = ReadFile(shaderFileName);
    const char* shaderSourceStr = shaderSource.c_str();
    glShaderSource(shader, 1, &shaderSourceStr, NULL);

    glCompileShader(shader);

    printShaderLog(shader);

    GLint isShaderCompiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
    if (isShaderCompiled != GL_TRUE)
    {
      printf("unable to compile shader %s\n", shaderFileName);
      return -1;
    }

    return shader;
  } // View::createShader

  bool View::initializeOpenGL()
  {
    _skinnedMeshDiffuseTextureProgramId = createProgram("shaders/SkinnedMeshShader.vert", "shaders/DiffuseTextureShader.frag");
    if (_skinnedMeshDiffuseTextureProgramId == -1)
    {
      return false;
    }
    _skinnedMeshDiffuseTextureProjectionViewModelMatrixId = glGetUniformLocation(_skinnedMeshDiffuseTextureProgramId, "projectionViewModel");
    _skinnedMeshDiffuseTexturePaletteId = glGetUniformLocation(_skinnedMeshDiffuseTextureProgramId, "palette");
    _skinnedMeshDiffuseTextureDiffuseTextureId = glGetUniformLocation(_skinnedMeshDiffuseTextureProgramId, "diffuseTexture");

    _skeletonProgramId = createProgram("shaders/SkeletonShader.vert", "shaders/FragmentShader.frag");
    if (_skeletonProgramId == -1)
    {
      return false;
    }
    _skeletonProjectionViewModelMatrixId = glGetUniformLocation(_skeletonProgramId, "projectionViewModel");
    _skeletonPaletteId = glGetUniformLocation(_skeletonProgramId, "palette");

    _uiProgramId = createProgram("shaders/UIShader.vert", "shaders/UIShader.frag");
    if (_uiProgramId == -1)
    {
      return false;
    }
    _uiTextureId = glGetUniformLocation(_uiProgramId, "uiTexture");

    _gridProgramId = createProgram("shaders/GridShader.vert", "shaders/GridShader.frag");
    if (_gridProgramId == -1)
    {
      return false;
    }
    _gridProjectionViewModelMatrixId = glGetUniformLocation(_gridProgramId, "projectionViewModel");

    _skinnedMeshWireFrameDiffuseTextureProgramId = createProgram("shaders/SkinnedMeshShader.vert", "shaders/WireFrameDiffuseTextureShader.frag");
    if (_skinnedMeshWireFrameDiffuseTextureProgramId == -1)
    {
      return false;
    }
    _skinnedMeshWireFrameDiffuseTextureProjectionViewModelMatrixId = glGetUniformLocation(_skinnedMeshWireFrameDiffuseTextureProgramId, "projectionViewModel");
    _skinnedMeshWireFrameDiffuseTexturePaletteId = glGetUniformLocation(_skinnedMeshWireFrameDiffuseTextureProgramId, "palette");
    _skinnedMeshWireFrameDiffuseTextureDiffuseTextureId = glGetUniformLocation(_skinnedMeshWireFrameDiffuseTextureProgramId, "diffuseTexture");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // TODO: what if there are dupes
    for (size_t i = 0; i < g_assetNames.size(); ++i)
    {
      // TODO: bad
      CE::Skeleton* skeleton = new CE::Skeleton();
      CE::Meshes* meshes = new CE::Meshes();
      CE::Animations* animations = new CE::Animations();
      CE::Textures* textures = new CE::Textures();

      CE::AssetImporter::ImportSkeletonMeshesAnimationsTextures(
        g_assetNames[i],
        *skeleton,
        *meshes,
        *animations,
        *textures);

      _meshComponents.push_back(new CE::MeshComponent(meshes, textures));
      _animationComponents.push_back(new CE::AnimationComponent(skeleton, animations, _eventSystem));
    }

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // TODO: Why do I need to bind the buffer immediately after generating it?
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);

    _diffuseTextureUnit = 0;
    glActiveTexture(GL_TEXTURE0 + _diffuseTextureUnit);
    glGenTextures(1, &_diffuseTextureID);
    glBindTexture(GL_TEXTURE_2D, _diffuseTextureID);

    // TODO: How much of this do I need to do here, versus every call?
    _paletteTextureUnit = 1;
    glGenBuffers(1, &_tbo);
    glBindBuffer(GL_TEXTURE_BUFFER, _tbo);
    glActiveTexture(GL_TEXTURE0 + _paletteTextureUnit);
    glGenTextures(1, &_paletteGenTex);
    glBindTexture(GL_TEXTURE_BUFFER, _paletteGenTex);
    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, _tbo);

    _uiTextureUnit = 2;
    glActiveTexture(GL_TEXTURE0 + _uiTextureUnit);
    glGenTextures(1, &_uiTextureID);
    glBindTexture(GL_TEXTURE_2D, _uiTextureID);

    return true;
  } // View::initializeOpenGL

  void View::printProgramLog(GLuint program)
  {
    if (!glIsProgram(program))
    {
      printf("GLuint %d is not a program\n", program);
      return;
    }

    GLint maxLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    char* infoLog = new char[maxLength];

    GLint infoLogLength = 0;
    glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
      printf("program log:\n");
      printf("%s\n", infoLog);
    }

    delete[] infoLog;
  } // View::printProgramLog

  void View::printShaderLog(GLuint shader)
  {
    if (!glIsShader(shader))
    {
      printf("GLuint %d is not a shader\n", shader);
      return;
    }

    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    char* infoLog = new char[maxLength];

    GLint infoLogLength = 0;
    glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
    if (infoLogLength > 0)
    {
      printf("shader log:\n");
      printf("%s\n", infoLog);
    }

    delete[] infoLog;
  } // View::printShaderLog

  void View::processEvent(SDL_Event& event)
  {
    CE::SdlEvent wrappedEvent;
    wrappedEvent.event = event;
    _eventSystem->DispatchEvent(wrappedEvent);

    // TODO: Haven't done focus events for Cef (see CefBrowserHost). Do I need these?
    switch (event.type)
    {
    case SDL_QUIT:
    {
      _quit = true;
      break;
    }

    case SDL_KEYDOWN:
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_q:
      {
        _eventSystem->EnqueueEvent(ToggleBindPoseEvent());
        break;
      }

      case SDLK_e:
      {
        SetRenderModeEvent setRenderModeEvent;
        setRenderModeEvent.mode = (renderMode() + 1) % 3;
        _eventSystem->EnqueueEvent(setRenderModeEvent);
        break;
      }
      }

      break;
    }
    }
  } // View::processEvent

  void View::render()
  {
//    printf("ticker:  %d   tm:  %6.4f \n", _fpsCounter->ticker, _fpsCounter->ticker_tm);
    //Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Is this required? Works without it?
    glBindVertexArray(_vao);

    glm::mat4 projection = glm::perspective(glm::quarter_pi<float>(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 10000.0f);
    glm::mat4 view = _camera->CreateViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projectionViewModel = projection * view * model;

    renderObjects(projectionViewModel);
    renderGrid(projectionViewModel);

    // Because of depth testing, and because the UI is currently rendered as
    // one giant texture the size of the screen instead of just its visible parts,
    // we must render the UI last. Otherwise, everything fails the depth test.
    //
    // Once we render only visible parts, we could render the UI first.
    // Then, everything behind the UI will fail the depth test, but for good reason.

    renderHUD(); // renders the javascript ui

    SDL_GL_SwapWindow(_window);
  } // View::render

  void View::renderObjects(const glm::mat4& projectionViewModel)
  {
    // render all object instances that fall within the viewport
    // ie: objects not in within the camera viewport should not be drawn
    //
    for (size_t i = 0; i < g_assetNames.size(); ++i)
    {
      renderMesh(*_meshComponents[i], *_animationComponents[i], projectionViewModel);
//      renderSkeleton(*_animationComponents[i], projectionViewModel);
    }
  } // View::renderObjects

  void View::renderGrid(const glm::mat4& projectionViewModel)
  {
    glUseProgram(_gridProgramId);

    struct GridVertex
    {
      glm::vec3 position;
      // color is hardcoded in GridShader.frag
    };

    unsigned stride = sizeof(GridVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(GridVertex, position)));
    glEnableVertexAttribArray(0);

    glUniformMatrix4fv(_gridProjectionViewModelMatrixId, 1, GL_FALSE, &projectionViewModel[0][0]);

    std::vector<GridVertex> gridVertices;
    std::vector<unsigned> gridIndices;

    int sideLength = 2000;
    int halfSideLength = sideLength / 2;
    int increment = 100;

    for (int i = -halfSideLength; i <= halfSideLength; i += increment)
    {
      GridVertex gridVertexMaxZ;
      gridVertexMaxZ.position = glm::vec3(i, 0, halfSideLength);
      gridVertices.push_back(gridVertexMaxZ);
      gridIndices.push_back(static_cast<unsigned>(gridIndices.size()));

      GridVertex gridVertexMinZ;
      gridVertexMinZ.position = glm::vec3(i, 0, -halfSideLength);
      gridVertices.push_back(gridVertexMinZ);
      gridIndices.push_back(static_cast<unsigned>(gridIndices.size()));


      GridVertex gridVertexMaxX;
      gridVertexMaxX.position = glm::vec3(halfSideLength, 0, i);
      gridVertices.push_back(gridVertexMaxX);
      gridIndices.push_back(static_cast<unsigned>(gridIndices.size()));

      GridVertex gridVertexMinX;
      gridVertexMinX.position = glm::vec3(-halfSideLength, 0, i);
      gridVertices.push_back(gridVertexMinX);
      gridIndices.push_back(static_cast<unsigned>(gridIndices.size()));
    }

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GridVertex) * gridVertices.size(), gridVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * gridIndices.size(), gridIndices.data(), GL_STATIC_DRAW);

    glLineWidth(1.f);
    glDrawElements(GL_LINES, (GLsizei)gridIndices.size(), GL_UNSIGNED_INT, NULL);

    glDisableVertexAttribArray(0);
  } // View::renderGrid

  void View::renderHUD()
  {
    glUseProgram(_uiProgramId);

    struct UIVertex
    {
      glm::vec2 position;
      float uv[2];
    };

    unsigned stride = sizeof(UIVertex);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(UIVertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(UIVertex, uv)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    std::vector<UIVertex> uiVertices;
    UIVertex uiVertex;

    uiVertex.position = glm::vec2(-1.f, -1.f);
    uiVertex.uv[0] = 0.f;
    uiVertex.uv[1] = 1.f;
    uiVertices.push_back(uiVertex);

    uiVertex.position = glm::vec2(1.f, -1.f);
    uiVertex.uv[0] = 1.f;
    uiVertex.uv[1] = 1.f;
    uiVertices.push_back(uiVertex);

    uiVertex.position = glm::vec2(-1.f, 1.f);
    uiVertex.uv[0] = 0.f;
    uiVertex.uv[1] = 0.f;
    uiVertices.push_back(uiVertex);

    uiVertex.position = glm::vec2(1.f, 1.f);
    uiVertex.uv[0] = 1.f;
    uiVertex.uv[1] = 0.f;
    uiVertices.push_back(uiVertex);

    std::vector<unsigned> uiIndices;
    uiIndices.push_back(0);
    uiIndices.push_back(1);
    uiIndices.push_back(2);
    uiIndices.push_back(1);
    uiIndices.push_back(3);
    uiIndices.push_back(2);

    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UIVertex) * uiVertices.size(), uiVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * uiIndices.size(), uiIndices.data(), GL_STATIC_DRAW);

    // TODO: How much of this has to be done every Draw() call?
    // TODO: double check these for UI
    glBindTexture(GL_TEXTURE_2D, _uiTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    const char* viewBuffer = _cefMain->GetViewBuffer();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, viewBuffer);
    const char* popupBuffer = _cefMain->GetPopupBuffer();
    if (popupBuffer != nullptr)
    {
      const CefRect& popupRect = _cefMain->GetPopupRect();
      glTexSubImage2D(GL_TEXTURE_2D, 0, popupRect.x, popupRect.y, popupRect.width, popupRect.height, GL_BGRA, GL_UNSIGNED_BYTE, popupBuffer);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    glUniform1i(_uiTextureId, _uiTextureUnit);

    glDrawElements(GL_TRIANGLES, (GLsizei)uiIndices.size(), GL_UNSIGNED_INT, NULL);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
  } // View::renderHUD

  void View::renderMesh(CE::MeshComponent& meshComponent, CE::AnimationComponent& animationComponent, const glm::mat4& projectionViewModel)
  {
    bool renderWireFrameOnly = _engine->GetRenderMode() == 3;
    GLuint activeProgramID = -1;
    GLuint activeProjectionViewModelMatrixID = -1;
    GLuint activePaletteID = -1;
    GLuint activeDiffuseTextureLocation = -1;

    if (renderWireFrameOnly)
    {
      activeProgramID = _skinnedMeshWireFrameDiffuseTextureProgramId;
      activeProjectionViewModelMatrixID = _skinnedMeshWireFrameDiffuseTextureProjectionViewModelMatrixId;
      activePaletteID = _skinnedMeshWireFrameDiffuseTexturePaletteId;
      activeDiffuseTextureLocation = _skinnedMeshWireFrameDiffuseTextureDiffuseTextureId;
    }
    else
    {
      activeProgramID = _skinnedMeshDiffuseTextureProgramId;
      activeProjectionViewModelMatrixID = _skinnedMeshDiffuseTextureProjectionViewModelMatrixId;
      activePaletteID = _skinnedMeshDiffuseTexturePaletteId;
      activeDiffuseTextureLocation = _skinnedMeshDiffuseTextureDiffuseTextureId;
    }

    glUseProgram(activeProgramID);

    unsigned int stride = sizeof(CE::Vertex1P1UV4J);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(CE::Vertex1P1UV4J, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(CE::Vertex1P1UV4J, uv)));
    glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, stride, reinterpret_cast<void*>(offsetof(CE::Vertex1P1UV4J, jointIndices)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(CE::Vertex1P1UV4J, jointWeights)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glUniformMatrix4fv(activeProjectionViewModelMatrixID, 1, GL_FALSE, &projectionViewModel[0][0]);

    if (_engine->IsRenderBindPose())
    {
      animationComponent.ResetMatrixPalette();
    }

    animationComponent.BindMatrixPalette(
      _paletteTextureUnit,
      _paletteGenTex,
      _tbo,
      activePaletteID);

    if (_engine->GetRenderMode() == 0 || _engine->GetRenderMode() == 2 || renderWireFrameOnly)
    {
      meshComponent.Draw(
        _vbo,
        _ibo,
        _diffuseTextureID,
        activeDiffuseTextureLocation,
        _diffuseTextureUnit);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
  } // View::renderMesh

  void View::renderSkeleton(CE::AnimationComponent& animationComponent, const glm::mat4& projectionViewModel)
  {
    glUseProgram(_skeletonProgramId);

    struct DebugSkeletonVertex
    {
      glm::vec3 position;
      glm::vec3 color;
      unsigned jointIndex;
    };

    unsigned stride = sizeof(DebugSkeletonVertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(DebugSkeletonVertex, position)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(DebugSkeletonVertex, color)));
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, stride, reinterpret_cast<void*>(offsetof(DebugSkeletonVertex, jointIndex)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glUniformMatrix4fv(_skeletonProjectionViewModelMatrixId, 1, GL_FALSE, &projectionViewModel[0][0]);

    if (_engine->IsRenderBindPose())
    {
      animationComponent.ResetMatrixPalette();
    }

    animationComponent.BindMatrixPalette(
      _paletteTextureUnit,
      _paletteGenTex,
      _tbo,
      _skeletonPaletteId);

    const CE::Skeleton* skeleton = animationComponent.GetSkeleton();// CE::SkeletonManager::Get().GetSkeleton(_assetNames[i]);

    std::vector<DebugSkeletonVertex> debugVertices;
    std::vector<unsigned> debugJointIndices;
    std::vector<unsigned> debugLineIndices;

    for (unsigned i = 0; i < skeleton->joints.size(); ++i)
    {
      glm::mat4 bindPose = glm::inverse(skeleton->joints[i].inverseBindPose);

      // todo: bad
      glm::vec3 scale;
      glm::quat orientation;
      glm::vec3 translation;
      glm::vec3 skew;
      glm::vec4 perspective;
      glm::decompose(
        bindPose,
        scale,
        orientation,
        translation,
        skew,
        perspective);

      DebugSkeletonVertex vertex;
      vertex.position = translation;
      vertex.color = glm::vec3(1.f, 0.f, 0.f);
      vertex.jointIndex = i;

      debugVertices.push_back(vertex);

      debugJointIndices.push_back(i);

      if (skeleton->joints[i].parentIndex != -1)
      {
        debugLineIndices.push_back(skeleton->joints[i].parentIndex);
        debugLineIndices.push_back(i);
      }
    }

    if (_engine->GetRenderMode() == 1 || _engine->GetRenderMode() == 2)
    {
      glBindBuffer(GL_ARRAY_BUFFER, _vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(DebugSkeletonVertex) * debugVertices.size(), debugVertices.data(), GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * debugJointIndices.size(), debugJointIndices.data(), GL_STATIC_DRAW);

      glPointSize(5.f);
      glDrawElements(GL_POINTS, (GLsizei)debugJointIndices.size(), GL_UNSIGNED_INT, NULL);

      for (unsigned i = 0; i < skeleton->joints.size(); ++i)
      {
        debugVertices[i].color = glm::vec3(1.f, 1.f, 0.f);
      }

      glBufferData(GL_ARRAY_BUFFER, sizeof(DebugSkeletonVertex) * debugVertices.size(), debugVertices.data(), GL_STATIC_DRAW);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * debugLineIndices.size(), debugLineIndices.data(), GL_STATIC_DRAW);

      glLineWidth(1.f);
      glDrawElements(GL_LINES, (GLsizei)debugLineIndices.size(), GL_UNSIGNED_INT, NULL);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  } // View::renderSkeleton

  View::View()
  {
  } // View [constructor]

  View::~View()
  {
    destroy();
  } // View [destructor]

  void View::destroy()
  {
    if (!_initialized) return;

    CE::MeshManager::Get().Destroy();
    CE::AnimationManager::Get().Destroy();
    CE::SkeletonManager::Get().Destroy();
    CE::TextureManager::Get().Destroy();

    _cefMain->StopCef();

    SDL_DestroyWindow(_window);
    _window = NULL;

    SDL_Quit();
    _initialized = false;
  } // View::destroy

#ifdef _WIN32
// osr_window_win.cc
  void WindowsMessageHook(
    void* userdata,
    void* hWnd,
    unsigned int message,
    Uint64 wParam,
    Sint64 lParam)
  {
    CE::WindowsMessageEvent event;
    event.userdata = 0; // userdata;
    event.hWnd = hWnd;
    event.message = message;
    event.wParam = wParam;
    event.lParam = lParam;
    ((EventSystem*)userdata)->DispatchEvent(event);
  }
#endif

  bool View::initialize(int argc, char* argv[])
  {
    if (_initialized)  return true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
      printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Enable MSAA.
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    _eventSystem = new EventSystem();
#ifdef _WIN32
    SDL_SetWindowsMessageHook(&WindowsMessageHook, (void*)_eventSystem);
#endif

    _window = SDL_CreateWindow(
      "Composite Engine",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      SCREEN_WIDTH,
      SCREEN_HEIGHT,
      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if (_window == NULL)
    {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      return false;
    }

    _context = SDL_GL_CreateContext(_window);

    // CefKeyEvent keyevent;
    // keyevent.unmodified_character = 0;

    if (_context == NULL)
    {
      printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
      return false;
    }

    // Initialize GLEW
    //glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
      printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
    }

    // Disable VSync
    if (SDL_GL_SetSwapInterval(0) < 0)
    {
      printf("Warning: Unable to set immediate updates for VSync! SDL_Error: %s\n", SDL_GetError());
    }

    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    //CE::MeshManager::Get().Initialize(_fbxImporter);
    //CE::AnimationManager::Get().Initialize(_fbxImporter);
    //CE::SkeletonManager::Get().Initialize(_fbxImporter);
    //CE::TextureManager::Get().Initialize(_stbiImporter);

    _engine = new CE::Engine(_eventSystem);
    _cefMain = new CE::CefMain(_eventSystem, _window);
    _fpsCounter = new CE::FpsCounter(_eventSystem);
    _camera = new CE::Camera(glm::vec3(0, 100, 700), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

    if (!initializeOpenGL())
    {
      printf("Unable to initialize OpenGL!\n");
      return false;
    }

    _editorCameraEventHandler = new CE::EditorCameraEventHandler(_eventSystem, _camera );

    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("GL_RENDERER: %s\n", renderer);
    printf("GL_VERSION: %s\n", version);
    // TODO: Doesn't work on my laptop's Intel HD Graphics 4000, which only supports up to OpenGL 4.0.

    // GLint components;
    // glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &components);
    // printf("GL_MAX_VERTEX_UNIFORM_COMPONENTS: %u\n", components);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    // Enable MSAA.
    glEnable(GL_MULTISAMPLE);

    // enable alpha blending (allows transparent textures)
    // https://gamedev.stackexchange.com/questions/29492/opengl-blending-gui-textures
    // https://www.opengl.org/archives/resources/faq/technical/transparency.htm
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!_cefMain->StartCef(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT))
    {
      printf("CEF failed to start!\n");
      return false;
    }

    return (_initialized = true);
  } // View::initialize

  int View::runLoop()
  {
    uint64_t currentTicks = SDL_GetPerformanceCounter();
    uint64_t previousTicks = 0;

    CE::RealTimeClock::Get().Initialize(currentTicks);
    CE::GameTimeClock::Get().Initialize(currentTicks);

    _quit = false;

    while (!_quit)
    {
      previousTicks = currentTicks;
      currentTicks = SDL_GetPerformanceCounter();

      uint64_t deltaTicks = currentTicks - previousTicks;
      CE::RealTimeClock::Get().Update(deltaTicks);
      CE::GameTimeClock::Get().Update(deltaTicks);

      SDL_Event event;

      while (SDL_PollEvent(&event) != 0)
      {
        processEvent(event);
      }

      update(CE::RealTimeClock::Get().GetDeltaSeconds(), CE::RealTimeClock::Get().GetCurrentTicks());
      render();
    }
    return 0;
  } // View::runLoop

  void View::stop()
  {
    _quit = true;
  } // View::stop

  void View::update(float dwDeltaSeconds, uint64_t currentTicks)
  {
    _editorCameraEventHandler->Update(dwDeltaSeconds);

    for (CE::AnimationComponent* animationComponent : _animationComponents)
    {
      animationComponent->Update(dwDeltaSeconds);
    }
    _fpsCounter->Update(dwDeltaSeconds);

    // TODO: Where does this go?
    _eventSystem->DispatchEvents(currentTicks);
  } // View::update

} // namespace: CE
