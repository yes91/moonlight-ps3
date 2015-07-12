/*
 * This file is part of Moonlight Embedded.
 *
 * Copyright (C) 2015 Iwan Timmer
 *
 * Moonlight is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Moonlight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Moonlight; if not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_SDL

#include "sdl.h"

#include "limelight-common/Limelight.h"

#include <stdbool.h>
#include <SDL.h>

static bool done;

void sdl_loop() {
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_ShowCursor(SDL_DISABLE);

  SDL_Event event;

  while(!done && SDL_WaitEvent(&event)) {
    int button = 0;
    switch (event.type) {
    case SDL_MOUSEMOTION:
      LiSendMouseMoveEvent(event.motion.xrel, event.motion.yrel);
      break;
    case SDL_MOUSEWHEEL:
      LiSendScrollEvent(event.wheel.y);
      break;
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEBUTTONDOWN:
      switch (event.button.button) {
      case SDL_BUTTON_LEFT:
        button = BUTTON_LEFT;
        break;
      case SDL_BUTTON_MIDDLE:
        button = BUTTON_MIDDLE;
        break;
      case SDL_BUTTON_RIGHT:
        button = BUTTON_RIGHT;
        break;
      }

      if (button != 0)
        LiSendMouseButtonEvent(event.type==SDL_MOUSEBUTTONDOWN?BUTTON_ACTION_PRESS:BUTTON_ACTION_RELEASE, button);

      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      button = event.key.keysym.sym;
      if (button >= (0x40000000 + 0x39) && button < (0x40000000 + sizeof(keyCodes)))
        button = keyCodes[button - 0x40000039];
      if (button >= 0x61)
        button -= 0x20;

      LiSendKeyboardEvent(0x80 << 8 | button, event.type==SDL_KEYDOWN?KEY_ACTION_DOWN:KEY_ACTION_UP, 0);
      break;
    case SDL_QUIT:
      done = true;
    }
  }
}

#endif /* HAVE_SDL */
