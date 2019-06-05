/*!
  @file       centralcore.cpp
  @brief      CentralCore class implementation

  @author     rmcinnis@solidice.com
  @date       june 01, 2019
  @par        copyright (c) 2019 ARK engineering
*/
#pragma warning(disable: 4786)
#pragma warning(disable: 4355)

#include "centralcore.h" 
#include "core/clock/RealTimeClock.h"
#include "core/clock/GameTimeClock.h"
#include "event/SdlEvent.h"
#include "core/EditorCameraEventHandler.h"
#include "event/ToggleBindPoseEvent.h"
#include "event/SetRenderModeEvent.h"

namespace CE
{
  CentralCore& core()
  {
    static CentralCore  instance;

    return instance;
  } // :: core

  void CentralCore::on_shutdown()
  {
    // post run loop event
    // 
    // to be used to shutdown networking, finailize data caches, stop threads, etc
  } // CentralCore :: on_shutdown

  void CentralCore::on_startup()
  {
    // pre run loop event
    // 
    // to be used to set up networking, initialize data caches, start threads, etc
  } // CentralCore :: on_startup

  CentralCore::CentralCore()
  {
  } // CentralCore [constructor]

  CentralCore :: ~CentralCore()
  {
  } // CentralCore [destructor]

  void CentralCore::destroy()
  {
    _view.destroy();
  } // CentralCore :: destroy

  bool CentralCore::initialize(int argc, char* argv[])
  {
    if (isInitialized()) return true;

    if (!_view.initialize(argc, argv))  return false;

    return true;
  } // CentralCore :: initialize

  int CentralCore::runLoop()
  {
    on_startup();

    int rc = _view.runLoop();

    on_shutdown();

    // cleanup
    destroy();
    return rc;
  } // CentralCore :: runLoop

} // namespace CE