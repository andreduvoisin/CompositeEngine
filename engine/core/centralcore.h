/*!
  @file       centralcore.h
  @brief      CentralCore class definition

  @author     rmcinnis@solidice.com
  @date       june 01, 2019
  @par        copyright (c) 2019 ARK engineering
*/
#pragma once

#include "View.h"

namespace CE
{

  class CentralCore
  {
  protected:
    bool           _quit = false ;
    View           _view;
    // Network       _net ;
    // ModelMgr      _models ;
    // World         _world ; // static objects and terrian
    // ObjectMgr     _objects ; 

    void           on_shutdown();
    void           on_startup();

  public:
                   CentralCore();
                  ~CentralCore();

    void           destroy();
    bool           initialize(int argc, char* argv[]);
    int            runLoop();

    // access methods
    bool           isInitialized() { return _view.isInitialized() ; }

  }; // class CentralCore

  CentralCore& core();

} // namespace: CE
